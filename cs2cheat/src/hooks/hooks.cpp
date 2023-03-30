#include <thread>

#include "hooks.hpp"

#include "../sdk/memory/memory.hpp"
#include "../api/hook/hook.hpp"

void CS2_HookInputAPI();
void CS2_UnhookInputAPI();
void CS2_HookGraphicsAPI();
void CS2_UnhookGraphicsAPI();
void CS2_HookGameFunctions();
void CS2_UnhookGameFunctions();

void hooks::Initialize() {
    g_funchookCtx = funchook_create();
    if (!g_funchookCtx) return;

    CS2_HookInputAPI();
    CS2_HookGameFunctions();
    CS2_HookGraphicsAPI();

    if (funchook_install(g_funchookCtx, 0) != FUNCHOOK_ERROR_SUCCESS) return;
    LOG("hooks::Initialize() successful.\n");
}

void hooks::Shutdown() {
    g_isShuttingDown = true;
    if (!g_funchookCtx) return;

    CS2_UnhookInputAPI();
    CS2_UnhookGameFunctions();
    CS2_UnhookGraphicsAPI();

    if (funchook_uninstall(g_funchookCtx, 0) != FUNCHOOK_ERROR_SUCCESS) return;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (funchook_destroy(g_funchookCtx) != FUNCHOOK_ERROR_SUCCESS) return;
}
