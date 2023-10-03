#include "pch.hpp"

#include <hooks/input/input_hooks.hpp>
#include <logger/logger.hpp>

#include <menu/menu.hpp>

#include <hooks/input/wndproc/wndproc_hooks.hpp>
#include <hooks/input/sdl/sdl_hooks.hpp>

void CInputHooks::Initialize() {
    SDK_LOG_PROLOGUE();

#ifdef _WIN32
    CWndProcHooks::Get().Initialize();
#else
    CSDLHooks::Get().Initialize();
#endif
}

void CInputHooks::Shutdown() {
    SDK_LOG_PROLOGUE();

    CMenu::Get().Shutdown();

#ifdef _WIN32
    CWndProcHooks::Get().Shutdown();
#endif
}
