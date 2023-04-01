#include <thread>

#include "../../../api/hook/hook.hpp"

#ifdef CS2_SDK_ENABLE_VULKAN_SUPPORT

// https://vulkan.lunarg.com/
#include <vulkan/vulkan.h>
#pragma comment(lib, "vulkan-1.lib")

#include <vector>

#include "../../../game/render/render.hpp"
#include "../../../console/console.hpp"

#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_win32.h>

static VkAllocationCallbacks* g_Allocator = NULL;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_FakeDevice = VK_NULL_HANDLE, g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t)-1;
static std::vector<VkQueueFamilyProperties> g_QueueFamilies;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
static uint32_t g_MinImageCount = 2;
static VkRenderPass g_RenderPass = VK_NULL_HANDLE;
static ImGui_ImplVulkanH_Frame g_Frames[8] = {};
static ImGui_ImplVulkanH_FrameSemaphores g_FrameSemaphores[8] = {};
static VkExtent2D g_ImageExtent = {};

static bool CreateDeviceVK();
static void CreateRenderTarget(VkDevice device, VkSwapchainKHR swapchain);
static void CleanupDeviceVulkan();
static void CleanupRenderTarget();
static void RenderImGui_Vulkan(VkQueue queue,
                               const VkPresentInfoKHR* pPresentInfo);
static bool DoesQueueSupportGraphic(VkQueue queue, VkQueue* pGraphicQueue);

static CHook<VkResult VKAPI_CALL(VkDevice, VkSwapchainKHR, uint64_t,
                                 VkSemaphore, VkFence, uint32_t*)>
    g_acquireNextImageKHR;
static VkResult VKAPI_CALL hkAcquireNextImageKHR(
    VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
    VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {
    g_Device = device;

    return g_acquireNextImageKHR.m_pOriginalFn(device, swapchain, timeout,
                                               semaphore, fence, pImageIndex);
}

static CHook<VkResult VKAPI_CALL(VkDevice, const VkAcquireNextImageInfoKHR*,
                                 uint32_t*)>
    g_acquireNextImage2KHR;
static VkResult VKAPI_CALL hkAcquireNextImage2KHR(
    VkDevice device, const VkAcquireNextImageInfoKHR* pAcquireInfo,
    uint32_t* pImageIndex) {
    g_Device = device;

    return g_acquireNextImage2KHR.m_pOriginalFn(device, pAcquireInfo,
                                                pImageIndex);
}

static CHook<VkResult VKAPI_CALL(VkQueue, const VkPresentInfoKHR*)>
    g_queuePresentKHR;
static VkResult VKAPI_CALL
hkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    RenderImGui_Vulkan(queue, pPresentInfo);

    return g_queuePresentKHR.m_pOriginalFn(queue, pPresentInfo);
}

static CHook<VkResult VKAPI_CALL(VkDevice, const VkSwapchainCreateInfoKHR*,
                                 const VkAllocationCallbacks*, VkSwapchainKHR*)>
    g_createSwapchainKHR;
static VkResult VKAPI_CALL hkCreateSwapchainKHR(
    VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
    const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
    CleanupRenderTarget();
    g_ImageExtent = pCreateInfo->imageExtent;

    return g_createSwapchainKHR.m_pOriginalFn(device, pCreateInfo, pAllocator,
                                              pSwapchain);
}

void CS2_HookVulkanGraphicsAPI() {
    CreateDeviceVK();

    void* fnAcquireNextImageKHR = reinterpret_cast<void*>(
        vkGetDeviceProcAddr(g_FakeDevice, "vkAcquireNextImageKHR"));
    void* fnAcquireNextImage2KHR = reinterpret_cast<void*>(
        vkGetDeviceProcAddr(g_FakeDevice, "vkAcquireNextImage2KHR"));
    void* fnQueuePresentKHR = reinterpret_cast<void*>(
        vkGetDeviceProcAddr(g_FakeDevice, "vkQueuePresentKHR"));
    void* fnCreateSwapchainKHR = reinterpret_cast<void*>(
        vkGetDeviceProcAddr(g_FakeDevice, "vkCreateSwapchainKHR"));

    if (g_FakeDevice) {
        vkDestroyDevice(g_FakeDevice, g_Allocator);
        g_FakeDevice = NULL;
    }

    g_acquireNextImageKHR.Hook(fnAcquireNextImageKHR,
                               HOOK_FUNCTION(hkAcquireNextImageKHR));
    g_acquireNextImage2KHR.Hook(fnAcquireNextImage2KHR,
                                HOOK_FUNCTION(hkAcquireNextImage2KHR));
    g_queuePresentKHR.Hook(fnQueuePresentKHR, HOOK_FUNCTION(hkQueuePresentKHR));
    g_createSwapchainKHR.Hook(fnCreateSwapchainKHR,
                              HOOK_FUNCTION(hkCreateSwapchainKHR));
}

void CS2_UnhookVulkanGraphicsAPI() {
    if (ImGui::GetCurrentContext()) {
        render::Shutdown();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        ImGuiIO& io = ImGui::GetIO();
        if (io.BackendRendererUserData) ImGui_ImplVulkan_Shutdown();

        if (io.BackendPlatformUserData) ImGui_ImplWin32_Shutdown();

        ImGui::DestroyContext();
    }

    CleanupDeviceVulkan();
}

static bool CreateDeviceVK() {
    // Create Vulkan Instance
    {
        VkInstanceCreateInfo create_info = {};
        constexpr const char* instance_extension = "VK_KHR_surface";

        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.enabledExtensionCount = 1;
        create_info.ppEnabledExtensionNames = &instance_extension;

        // Create Vulkan Instance without any debug feature
        vkCreateInstance(&create_info, g_Allocator, &g_Instance);
    }

    // Select GPU
    {
        uint32_t gpu_count;
        vkEnumeratePhysicalDevices(g_Instance, &gpu_count, NULL);
        IM_ASSERT(gpu_count > 0);

        VkPhysicalDevice* gpus =
            new VkPhysicalDevice[sizeof(VkPhysicalDevice) * gpu_count];
        vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus);

        // If a number >1 of GPUs got reported, find discrete GPU if present, or
        // use first one available. This covers most common cases
        // (multi-gpu/integrated+dedicated graphics). Handling more complicated
        // setups (multiple dedicated GPUs) is out of scope of this sample.
        int use_gpu = 0;
        for (int i = 0; i < (int)gpu_count; ++i) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(gpus[i], &properties);
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                use_gpu = i;
                break;
            }
        }

        g_PhysicalDevice = gpus[use_gpu];
        delete[] gpus;
    }

    // Select graphics queue family
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count,
                                                 NULL);
        g_QueueFamilies.resize(count);
        vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count,
                                                 g_QueueFamilies.data());
        for (uint32_t i = 0; i < count; ++i) {
            if (g_QueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                g_QueueFamily = i;
                break;
            }
        }
        IM_ASSERT(g_QueueFamily != (uint32_t)-1);
    }

    // Create Logical Device (with 1 queue)
    {
        constexpr const char* device_extension = "VK_KHR_swapchain";
        constexpr const float queue_priority = 1.0f;

        VkDeviceQueueCreateInfo queue_info = {};
        queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex = g_QueueFamily;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &queue_priority;

        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = 1;
        create_info.pQueueCreateInfos = &queue_info;
        create_info.enabledExtensionCount = 1;
        create_info.ppEnabledExtensionNames = &device_extension;

        vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator,
                       &g_FakeDevice);
    }

    return true;
}

static void CreateRenderTarget(VkDevice device, VkSwapchainKHR swapchain) {
    uint32_t uImageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &uImageCount, NULL);

    VkImage backbuffers[8] = {};
    vkGetSwapchainImagesKHR(device, swapchain, &uImageCount, backbuffers);

    for (uint32_t i = 0; i < uImageCount; ++i) {
        g_Frames[i].Backbuffer = backbuffers[i];

        ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
        ImGui_ImplVulkanH_FrameSemaphores* fsd = &g_FrameSemaphores[i];
        {
            VkCommandPoolCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            info.queueFamilyIndex = g_QueueFamily;

            vkCreateCommandPool(device, &info, g_Allocator, &fd->CommandPool);
        }
        {
            VkCommandBufferAllocateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            info.commandPool = fd->CommandPool;
            info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            info.commandBufferCount = 1;

            vkAllocateCommandBuffers(device, &info, &fd->CommandBuffer);
        }
        {
            VkFenceCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            vkCreateFence(device, &info, g_Allocator, &fd->Fence);
        }
        {
            VkSemaphoreCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            vkCreateSemaphore(device, &info, g_Allocator,
                              &fsd->ImageAcquiredSemaphore);
            vkCreateSemaphore(device, &info, g_Allocator,
                              &fsd->RenderCompleteSemaphore);
        }
    }

    // Create the Render Pass
    {
        VkAttachmentDescription attachment = {};
        attachment.format = VK_FORMAT_B8G8R8A8_UNORM;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment = {};
        color_attachment.attachment = 0;
        color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment;

        VkRenderPassCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        info.attachmentCount = 1;
        info.pAttachments = &attachment;
        info.subpassCount = 1;
        info.pSubpasses = &subpass;

        vkCreateRenderPass(device, &info, g_Allocator, &g_RenderPass);
    }

    // Create The Image Views
    {
        VkImageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format = VK_FORMAT_B8G8R8A8_UNORM;

        info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;

        for (uint32_t i = 0; i < uImageCount; ++i) {
            ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
            info.image = fd->Backbuffer;

            vkCreateImageView(device, &info, g_Allocator, &fd->BackbufferView);
        }
    }

    // Create Framebuffer
    {
        VkImageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = g_RenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.layers = 1;

        for (uint32_t i = 0; i < uImageCount; ++i) {
            ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
            attachment[0] = fd->BackbufferView;

            vkCreateFramebuffer(device, &info, g_Allocator, &fd->Framebuffer);
        }
    }

    if (!g_DescriptorPool)  // Create Descriptor Pool.
    {
        constexpr VkDescriptorPoolSize pool_sizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        vkCreateDescriptorPool(device, &pool_info, g_Allocator,
                               &g_DescriptorPool);
    }
}

static void CleanupRenderTarget() {
    for (uint32_t i = 0; i < IM_ARRAYSIZE(g_Frames); ++i) {
        if (g_Frames[i].Fence) {
            vkDestroyFence(g_Device, g_Frames[i].Fence, g_Allocator);
            g_Frames[i].Fence = VK_NULL_HANDLE;
        }
        if (g_Frames[i].CommandBuffer) {
            vkFreeCommandBuffers(g_Device, g_Frames[i].CommandPool, 1,
                                 &g_Frames[i].CommandBuffer);
            g_Frames[i].CommandBuffer = VK_NULL_HANDLE;
        }
        if (g_Frames[i].CommandPool) {
            vkDestroyCommandPool(g_Device, g_Frames[i].CommandPool,
                                 g_Allocator);
            g_Frames[i].CommandPool = VK_NULL_HANDLE;
        }
        if (g_Frames[i].BackbufferView) {
            vkDestroyImageView(g_Device, g_Frames[i].BackbufferView,
                               g_Allocator);
            g_Frames[i].BackbufferView = VK_NULL_HANDLE;
        }
        if (g_Frames[i].Framebuffer) {
            vkDestroyFramebuffer(g_Device, g_Frames[i].Framebuffer,
                                 g_Allocator);
            g_Frames[i].Framebuffer = VK_NULL_HANDLE;
        }
    }

    for (uint32_t i = 0; i < IM_ARRAYSIZE(g_FrameSemaphores); ++i) {
        if (g_FrameSemaphores[i].ImageAcquiredSemaphore) {
            vkDestroySemaphore(g_Device,
                               g_FrameSemaphores[i].ImageAcquiredSemaphore,
                               g_Allocator);
            g_FrameSemaphores[i].ImageAcquiredSemaphore = VK_NULL_HANDLE;
        }
        if (g_FrameSemaphores[i].RenderCompleteSemaphore) {
            vkDestroySemaphore(g_Device,
                               g_FrameSemaphores[i].RenderCompleteSemaphore,
                               g_Allocator);
            g_FrameSemaphores[i].RenderCompleteSemaphore = VK_NULL_HANDLE;
        }
    }
}

static void CleanupDeviceVulkan() {
    CleanupRenderTarget();

    if (g_DescriptorPool) {
        vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);
        g_DescriptorPool = NULL;
    }
    if (g_Instance) {
        vkDestroyInstance(g_Instance, g_Allocator);
        g_Instance = NULL;
    }

    g_ImageExtent = {};
    g_Device = NULL;
}

static void RenderImGui_Vulkan(VkQueue queue,
                               const VkPresentInfoKHR* pPresentInfo) {
    if (!ImGui::GetCurrentContext() || !g_Device || ::g_isShuttingDown) return;

    VkQueue graphicQueue = VK_NULL_HANDLE;
    const bool queueSupportsGraphic =
        DoesQueueSupportGraphic(queue, &graphicQueue);

    for (uint32_t i = 0; i < pPresentInfo->swapchainCount; ++i) {
        VkSwapchainKHR swapchain = pPresentInfo->pSwapchains[i];
        if (g_Frames[0].Framebuffer == VK_NULL_HANDLE) {
            CreateRenderTarget(g_Device, swapchain);
        }

        ImGui_ImplVulkanH_Frame* fd = &g_Frames[pPresentInfo->pImageIndices[i]];
        ImGui_ImplVulkanH_FrameSemaphores* fsd =
            &g_FrameSemaphores[pPresentInfo->pImageIndices[i]];
        {
            vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, ~0ull);
            vkResetFences(g_Device, 1, &fd->Fence);
        }
        {
            vkResetCommandBuffer(fd->CommandBuffer, 0);

            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(fd->CommandBuffer, &info);
        }
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = g_RenderPass;
            info.framebuffer = fd->Framebuffer;
            if (g_ImageExtent.width == 0 || g_ImageExtent.height == 0) {
                // We don't know the window size the first time. So we just set
                // it to 4K.
                info.renderArea.extent.width = 3840;
                info.renderArea.extent.height = 2160;
            } else {
                info.renderArea.extent = g_ImageExtent;
            }

            vkCmdBeginRenderPass(fd->CommandBuffer, &info,
                                 VK_SUBPASS_CONTENTS_INLINE);
        }

        if (!ImGui::GetIO().BackendRendererUserData) {
            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = g_Instance;
            init_info.PhysicalDevice = g_PhysicalDevice;
            init_info.Device = g_Device;
            init_info.QueueFamily = g_QueueFamily;
            init_info.Queue = graphicQueue;
            init_info.PipelineCache = g_PipelineCache;
            init_info.DescriptorPool = g_DescriptorPool;
            init_info.Subpass = 0;
            init_info.MinImageCount = g_MinImageCount;
            init_info.ImageCount = g_MinImageCount;
            init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            init_info.Allocator = g_Allocator;

            render::Initialize();

            ImGui_ImplVulkan_Init(&init_info, g_RenderPass);
            ImGui_ImplVulkan_CreateFontsTexture(fd->CommandBuffer);
        }

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        render::NewFrame();

        ImGui::Render();

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                        fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        vkEndCommandBuffer(fd->CommandBuffer);

        uint32_t waitSemaphoresCount =
            i == 0 ? pPresentInfo->waitSemaphoreCount : 0;
        if (waitSemaphoresCount == 0 && !queueSupportsGraphic) {
            constexpr VkPipelineStageFlags stages_wait =
                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
            {
                VkSubmitInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

                info.pWaitDstStageMask = &stages_wait;

                info.signalSemaphoreCount = 1;
                info.pSignalSemaphores = &fsd->RenderCompleteSemaphore;

                vkQueueSubmit(queue, 1, &info, VK_NULL_HANDLE);
            }
            {
                VkSubmitInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                info.commandBufferCount = 1;
                info.pCommandBuffers = &fd->CommandBuffer;

                info.pWaitDstStageMask = &stages_wait;
                info.waitSemaphoreCount = 1;
                info.pWaitSemaphores = &fsd->RenderCompleteSemaphore;

                info.signalSemaphoreCount = 1;
                info.pSignalSemaphores = &fsd->ImageAcquiredSemaphore;

                vkQueueSubmit(graphicQueue, 1, &info, fd->Fence);
            }
        } else {
            std::vector<VkPipelineStageFlags> stages_wait(
                waitSemaphoresCount, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;

            info.pWaitDstStageMask = stages_wait.data();
            info.waitSemaphoreCount = waitSemaphoresCount;
            info.pWaitSemaphores = pPresentInfo->pWaitSemaphores;

            info.signalSemaphoreCount = 1;
            info.pSignalSemaphores = &fsd->ImageAcquiredSemaphore;

            vkQueueSubmit(graphicQueue, 1, &info, fd->Fence);
        }
    }
}

static bool DoesQueueSupportGraphic(VkQueue queue, VkQueue* pGraphicQueue) {
    for (uint32_t i = 0; i < g_QueueFamilies.size(); ++i) {
        const VkQueueFamilyProperties& family = g_QueueFamilies[i];
        for (uint32_t j = 0; j < family.queueCount; ++j) {
            VkQueue it = VK_NULL_HANDLE;
            vkGetDeviceQueue(g_Device, i, j, &it);

            if (pGraphicQueue && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                if (*pGraphicQueue == VK_NULL_HANDLE) {
                    *pGraphicQueue = it;
                }
            }

            if (queue == it && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                return true;
            }
        }
    }

    return false;
}
#endif
