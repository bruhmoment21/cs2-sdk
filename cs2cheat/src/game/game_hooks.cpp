#include "../sdk/interfaces/interfaces.hpp"
#include "../api/hook/hook.hpp"

#include "skins/skin_changer.hpp"
#include "menu/menu.hpp"

static CHook<bool __fastcall(void*)> g_mouseInputEnabled;
static bool __fastcall hkMouseInputEnabled(void* rcx) {
    if (menu::IsOpen()) return false;
    return g_mouseInputEnabled.m_pOriginalFn(rcx);
}

static CHook<void __fastcall(void*, int)> g_frameStageNotify;
static void __fastcall hkFrameStageNotify(void* rcx, int frameStage) {
    if (5 <= frameStage && frameStage <= 6) skin_changer::Run();
    return g_frameStageNotify.m_pOriginalFn(rcx, frameStage);
}

void CS2_HookGameFunctions() {
    g_mouseInputEnabled.Hook(memory::fnMouseInputEnabled,
                             HOOK_FUNCTION(hkMouseInputEnabled));
    g_frameStageNotify.HookVirtual(interfaces::pClient, 29,
                                   HOOK_FUNCTION(hkFrameStageNotify));
}

void CS2_UnhookGameFunctions() { skin_changer::Shutdown(); }
