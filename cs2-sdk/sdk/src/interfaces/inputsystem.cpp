#include "pch.hpp"

#include <interfaces/inputsystem.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

CInputSystem* CInputSystem::Get() {
    static const auto inst = CMemory::GetInterface(CConstants::INPUTSYSTEM_LIB, "InputSystemVersion001");
    return inst.Get<CInputSystem*>();
}

bool CInputSystem::IsRelativeMouseMode() {
    // Offset in 'CInputSystem::SetRelativeMouseMode'.
    // Function is right above 'CInputSystem::DebugSpew'.

    return CPointer(this).GetField<bool>(0x4F);
}

void* CInputSystem::GetSDLWindow() {
    // Offset in 'CInputSystem::DebugSpew'.
    // xref: "SDL clip window state on 0x%p is %s\n".

    // Offset in 'CInputSystem::SetCursorClip'.
    // xref: "SetCursorClip:  %s SDL_SetWindowGrab on 0x%p (%s) %s\n".

    return CPointer(this).GetField<void*>(platform::Constant(0x2678, 0x26D8));
}
