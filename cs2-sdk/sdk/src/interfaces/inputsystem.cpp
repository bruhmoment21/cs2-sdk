#include "pch.hpp"

#include <interfaces/inputsystem.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

CInputSystem* CInputSystem::Get() {
    static const auto inst = CMemory::GetInterface(CConstants::INPUTSYSTEM_LIB, "InputSystemVersion001");
    return inst.Get<CInputSystem*>();
}

void* CInputSystem::GetSDLWindow() {
    // Offset in 'CInputSystem::DebugSpew'.
    // xref: "SDL says cursor is %s and is set to 0x%p\n".

    // Offset in 'CInputSystem::SetCursorClip'.
    // xref: "SetCursorClip:  %s SDL_SetWindowGrab on 0x%p (%s) %s\n".

    return CPointer(this).GetField<void*>(0x2648);
}
