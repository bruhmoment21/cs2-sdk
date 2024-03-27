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

    // As of some time this cannot be called here.
    //  I think SDL3 now wants you to call 'SetRelativeMouseMode'
    //  from the game's thread and not this thread.
    //  'CInputHooks::Shutdown' gets called by the thread that unloads the module.
    //
    // CMenu::Get().Shutdown();

#ifdef _WIN32
    CWndProcHooks::Get().Shutdown();
#endif
}
