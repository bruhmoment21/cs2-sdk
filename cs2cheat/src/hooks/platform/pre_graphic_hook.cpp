#include "../../utils/console/console.hpp"
#include "../../utils/utils.hpp"

void CS2_HookVulkanGraphicsAPI();
void CS2_UnhookVulkanGraphicsAPI();
void CS2_HookDX11GraphicsAPI();
void CS2_UnhookDX11GraphicsAPI();

void CS2_HookGraphicsAPI() {
    if (utils::IsUsingVulkan()) {
#ifdef CS2_SDK_ENABLE_VULKAN_SUPPORT
        CS2_HookVulkanGraphicsAPI();
#else
        LOG("Cannot hook Vulkan API because SDK was not built with "
            "CS2_SDK_ENABLE_VULKAN_SUPPORT\n");
#endif
    } else {
        CS2_HookDX11GraphicsAPI();
    }
}

void CS2_UnhookGraphicsAPI() {
    if (utils::IsUsingVulkan()) {
#ifdef CS2_SDK_ENABLE_VULKAN_SUPPORT
        CS2_UnhookVulkanGraphicsAPI();
#endif
    } else {
        CS2_UnhookDX11GraphicsAPI();
    }
}
