#include "pch.hpp"

#include <sdl/sdl.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

namespace sdl {
    bool SetWindowRelativeMouseMode(void* window, int enabled) {
        static const auto fn = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_SetWindowRelativeMouseMode");
        return fn.Call<decltype(&SetWindowRelativeMouseMode)>(window, enabled);
    }

    bool SetWindowMouseGrab(void* window, int grabbed) {
        static const auto fn = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_SetWindowMouseGrab");
        return fn.Call<decltype(&SetWindowMouseGrab)>(window, grabbed);
    }

    void WarpMouseInWindow(void* window, float x, float y) {
        static const auto fn = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_WarpMouseInWindow");
        return fn.Call<decltype(&WarpMouseInWindow)>(window, x, y);
    }
}  // namespace sdl
