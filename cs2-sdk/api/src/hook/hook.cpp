#include "pch.hpp"

#include <hook/hook.hpp>
#include <logger/logger.hpp>

#include <virtual/virtual.hpp>

funchook_t* CHook::s_FuncHookContext;

void CHook::Hook(void* target, void* hook, const char* name) {
    // Even though the function is called "Hook"
    // its purpose is to prepare the hooking.
    // Hooks are installed by funchook_install in hooks.cpp.

    if (!s_FuncHookContext) {
        return CLogger::Log("[hook] CHook::s_FuncHookContext not initialized!");
    }

    if (m_Original) {
        return CLogger::Log("[hook] tried rehooking {}. (Debug?)", name);
    }

    if (!target) {
        return CLogger::Log("[hook] tried hooking null to redirect to {}. (Debug?)", name);
    }

    m_Original = target;
    if (funchook_prepare(s_FuncHookContext, &m_Original, hook) != FUNCHOOK_ERROR_SUCCESS) {
        m_Original = nullptr;
        CLogger::Log("[hook] failed to prepare hooking {}. {}", name, funchook_error_message(s_FuncHookContext));
    } else {
        CLogger::Log("[hook] prepared {} [ {} -> {} ]", name, target, hook);
    }
}

void CHook::VHook(void* base, int index, void* hook, const char* name) { Hook(vt::GetMethod(base, index).Get<void*>(), hook, name); }
