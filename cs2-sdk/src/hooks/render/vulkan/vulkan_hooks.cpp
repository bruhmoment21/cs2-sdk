#include "pch.hpp"

#include <hooks/render/vulkan/vulkan_hooks.hpp>
#include <logger/logger.hpp>

// [!] Windows only.
// Make sure you have the Vulkan SDK installed and
// included 'imgui_impl_vulkan.cpp' and 'imgui_impl_vulkan.h' in project.
// #define SDK_ENABLE_VULKAN_HOOK

#if !defined(SDK_ENABLE_VULKAN_HOOK) && defined(__linux__)
#define SDK_ENABLE_VULKAN_HOOK
#endif

#ifdef SDK_ENABLE_VULKAN_HOOK
#include <renderer/renderer.hpp>

#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui.h>

#include <hook/hook.hpp>

#include <vulkan/vulkan.h>

#ifdef _WIN32
#pragma comment(lib, "vulkan-1.lib")
#endif

static constexpr uint32_t g_MinImageCount = 2;

static VkAllocationCallbacks* g_Allocator = NULL;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_FakeDevice = VK_NULL_HANDLE, g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = (uint32_t)-1;
static std::vector<VkQueueFamilyProperties> g_QueueFamilies;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;
static VkRenderPass g_RenderPass = VK_NULL_HANDLE;
static ImGui_ImplVulkanH_Frame g_Frames[8] = {};

static VkQueue GetGraphicQueue() {
    for (uint32_t i = 0; i < g_QueueFamilies.size(); ++i) {
        const VkQueueFamilyProperties& family = g_QueueFamilies[i];
        for (uint32_t j = 0; j < family.queueCount; ++j) {
            VkQueue it = VK_NULL_HANDLE;
            vkGetDeviceQueue(g_Device, i, j, &it);

            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                return it;
            }
        }
    }

    return VK_NULL_HANDLE;
}

static void CreateDevice() {
    SDK_LOG_PROLOGUE();

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

        VkPhysicalDevice* gpus = new VkPhysicalDevice[sizeof(VkPhysicalDevice) * gpu_count];
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
        vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, NULL);
        g_QueueFamilies.resize(count);
        vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, g_QueueFamilies.data());
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

        vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_FakeDevice);
    }
}

static void CreateRenderTarget(VkDevice device, VkSwapchainKHR swapchain) {
    SDK_LOG_PROLOGUE();

    uint32_t uImageCount;
    vkGetSwapchainImagesKHR(device, swapchain, &uImageCount, NULL);

    VkImage backbuffers[8] = {};
    vkGetSwapchainImagesKHR(device, swapchain, &uImageCount, backbuffers);

    for (uint32_t i = 0; i < uImageCount; ++i) {
        g_Frames[i].Backbuffer = backbuffers[i];

        ImGui_ImplVulkanH_Frame* fd = &g_Frames[i];
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
        constexpr VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
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

        vkCreateDescriptorPool(device, &pool_info, g_Allocator, &g_DescriptorPool);
    }
}

static void CleanupRenderTarget() {
    SDK_LOG_PROLOGUE();

    for (uint32_t i = 0; i < IM_ARRAYSIZE(g_Frames); ++i) {
        if (g_Frames[i].CommandBuffer) {
            vkFreeCommandBuffers(g_Device, g_Frames[i].CommandPool, 1, &g_Frames[i].CommandBuffer);
            g_Frames[i].CommandBuffer = VK_NULL_HANDLE;
        }
        if (g_Frames[i].CommandPool) {
            vkDestroyCommandPool(g_Device, g_Frames[i].CommandPool, g_Allocator);
            g_Frames[i].CommandPool = VK_NULL_HANDLE;
        }
        if (g_Frames[i].BackbufferView) {
            vkDestroyImageView(g_Device, g_Frames[i].BackbufferView, g_Allocator);
            g_Frames[i].BackbufferView = VK_NULL_HANDLE;
        }
        if (g_Frames[i].Framebuffer) {
            vkDestroyFramebuffer(g_Device, g_Frames[i].Framebuffer, g_Allocator);
            g_Frames[i].Framebuffer = VK_NULL_HANDLE;
        }
    }
}

static void CleanupDevice() {
    SDK_LOG_PROLOGUE();

    CleanupRenderTarget();

    if (g_DescriptorPool) {
        vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);
        g_DescriptorPool = NULL;
    }
    if (g_Instance) {
        vkDestroyInstance(g_Instance, g_Allocator);
        g_Instance = NULL;
    }

    g_Device = NULL;
}

static void RenderImGui(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    if (!ImGui::GetCurrentContext() || !g_Device) return;

    VkQueue graphicQueue = GetGraphicQueue();
    if (!graphicQueue) {
        return CLogger::Log("[vk] No queue that has VK_QUEUE_GRAPHICS_BIT has been found!");
    }

    for (uint32_t i = 0; i < pPresentInfo->swapchainCount; ++i) {
        VkSwapchainKHR swapchain = pPresentInfo->pSwapchains[i];
        if (g_Frames[0].Framebuffer == VK_NULL_HANDLE) {
            CreateRenderTarget(g_Device, swapchain);
        }

        ImGui_ImplVulkanH_Frame* fd = &g_Frames[pPresentInfo->pImageIndices[i]];
        {
            vkResetCommandBuffer(fd->CommandBuffer, 0);

            VkCommandBufferBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(fd->CommandBuffer, &info);
        }
        {
            auto [width, height] = ImGui::GetIO().DisplaySize;

            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = g_RenderPass;
            info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = static_cast<uint32_t>(width);
            info.renderArea.extent.height = static_cast<uint32_t>(height);

            vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
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

            CRenderer::Get().Initialize();

            ImGui_ImplVulkan_Init(&init_info, g_RenderPass);
        }

        ImGui_ImplVulkan_NewFrame();

#ifdef _WIN32
        ImGui_ImplWin32_NewFrame();
#else
        ImGui_ImplSDL3_NewFrame();
#endif

        ImGui::NewFrame();

        CRenderer::Get().NewFrame();

        ImGui::Render();

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

        // Submit command buffer
        vkCmdEndRenderPass(fd->CommandBuffer);
        vkEndCommandBuffer(fd->CommandBuffer);

        constexpr VkPipelineStageFlags stages_wait = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
        {
            VkSubmitInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            info.pWaitDstStageMask = &stages_wait;

            info.commandBufferCount = 1;
            info.pCommandBuffers = &fd->CommandBuffer;

            vkQueueSubmit(graphicQueue, 1, &info, VK_NULL_HANDLE);
        }
    }
}

static CHook g_AcquireNextImageKHR;
static VkResult VKAPI_CALL hkAcquireNextImageKHR(VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore,
                                                 VkFence fence, uint32_t* pImageIndex) {
    g_Device = device;

    return g_AcquireNextImageKHR.CallOriginal<VkResult>(device, swapchain, timeout, semaphore, fence, pImageIndex);
}

static CHook g_QueuePresentKHR;
static VkResult VKAPI_CALL hkQueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    RenderImGui(queue, pPresentInfo);

    return g_QueuePresentKHR.CallOriginal<VkResult>(queue, pPresentInfo);
}

static CHook g_CreateSwapchainKHR;
static VkResult VKAPI_CALL hkCreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
                                                const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
    CleanupRenderTarget();

    return g_CreateSwapchainKHR.CallOriginal<VkResult>(device, pCreateInfo, pAllocator, pSwapchain);
}
#endif

void CVulkanHooks::Initialize() {
    SDK_LOG_PROLOGUE();

#ifdef SDK_ENABLE_VULKAN_HOOK
    CreateDevice();

    if (!g_FakeDevice) {
        return CLogger::Log("[vk] Couldn't create a fake device!");
    }

    auto acquireNextImageKHR = vkGetDeviceProcAddr(g_FakeDevice, "vkAcquireNextImageKHR");
    auto queuePresentKHR = vkGetDeviceProcAddr(g_FakeDevice, "vkQueuePresentKHR");
    auto createSwapchainKHR = vkGetDeviceProcAddr(g_FakeDevice, "vkCreateSwapchainKHR");

    vkDestroyDevice(g_FakeDevice, g_Allocator);

    g_AcquireNextImageKHR.Hook(reinterpret_cast<void*>(acquireNextImageKHR), SDK_HOOK(hkAcquireNextImageKHR));
    g_QueuePresentKHR.Hook(reinterpret_cast<void*>(queuePresentKHR), SDK_HOOK(hkQueuePresentKHR));
    g_CreateSwapchainKHR.Hook(reinterpret_cast<void*>(createSwapchainKHR), SDK_HOOK(hkCreateSwapchainKHR));
#else
    CLogger::Log("[vk] Library wasn't built with SDK_ENABLE_VULKAN_HOOK flag.");
#endif
}

CVulkanHooks::~CVulkanHooks() {
    SDK_LOG_PROLOGUE();

#ifdef SDK_ENABLE_VULKAN_HOOK
    if (ImGui::GetCurrentContext()) {
        ImGuiIO& io = ImGui::GetIO();

        if (io.BackendRendererUserData) {
            ImGui_ImplVulkan_Shutdown();
        }

        if (io.BackendPlatformUserData) {
#ifdef _WIN32
            ImGui_ImplWin32_Shutdown();
#else
            ImGui_ImplSDL3_Shutdown();
#endif
        }

        ImGui::DestroyContext();
    }

    CleanupDevice();
#endif
}
