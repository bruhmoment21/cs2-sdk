#include "pch.hpp"

#include <hooks/render/render_hooks.hpp>
#include <logger/logger.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

#include <hooks/render/directx11/directx11_hooks.hpp>
#include <hooks/render/vulkan/vulkan_hooks.hpp>

void CRenderHooks::Initialize() {
    SDK_LOG_PROLOGUE();

#ifdef _WIN32
    if (!RunningVulkan()) {
        return CDirectX11Hooks::Get().Initialize();
    }
#endif

    return CVulkanHooks::Get().Initialize();
}

bool CRenderHooks::RunningVulkan() { return CMemory::GetModule(CConstants::RENDERSYSTEMVULKAN_LIB) != nullptr; }
