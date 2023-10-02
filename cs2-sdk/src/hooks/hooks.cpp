#include "pch.hpp"

#include <hooks/hooks.hpp>
#include <logger/logger.hpp>

#include <hooks/render/render_hooks.hpp>
#include <hooks/input/input_hooks.hpp>
#include <hooks/game/game_hooks.hpp>

#include <hook/hook.hpp>

void CHooks::Initialize() {
    SDK_LOG_PROLOGUE();

    CHook::s_FuncHookContext = funchook_create();
    if (!CHook::s_FuncHookContext) {
        return CLogger::Log("[fh] funchook_create() failed!");
    }

    CInputHooks::Get().Initialize();
    CRenderHooks::Get().Initialize();
    CGameHooks::Get().Initialize();

    if (funchook_install(CHook::s_FuncHookContext, 0) != FUNCHOOK_ERROR_SUCCESS) {
        return CLogger::Log("[fh] funchook_install() failed!");
    }
}

void CHooks::Shutdown() {
    SDK_LOG_PROLOGUE();

    CInputHooks::Get().Shutdown();

    if (!CHook::s_FuncHookContext) return;

    if (funchook_uninstall(CHook::s_FuncHookContext, 0) != FUNCHOOK_ERROR_SUCCESS) {
        return CLogger::Log("[fh] funchook_uninstall() failed!");
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

CHooks::~CHooks() {
    SDK_LOG_PROLOGUE();

    if (!CHook::s_FuncHookContext) return;

    if (funchook_destroy(CHook::s_FuncHookContext) != FUNCHOOK_ERROR_SUCCESS) {
        CLogger::Log("[fh] funchook_destroy() failed!");
    }
}
