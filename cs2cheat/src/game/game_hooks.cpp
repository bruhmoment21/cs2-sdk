#include "../sdk/interfaces/interfaces.hpp"
#include "../api/hook/hook.hpp"

#include "menu/menu.hpp"

static CHook<bool __fastcall(void*)> g_mouseInputEnabled;
static bool __fastcall hkMouseInputEnabled(void* rcx) {
    if (menu::IsOpen()) return false;
    return g_mouseInputEnabled.m_pOriginalFn(rcx);
}

void CS2_HookGameFunctions() {
    g_mouseInputEnabled.Hook(memory::fnMouseInputEnabled,
                             HOOK_FUNCTION(hkMouseInputEnabled));
}

void CS2_UnhookGameFunctions() { menu::Toggle(false); }
