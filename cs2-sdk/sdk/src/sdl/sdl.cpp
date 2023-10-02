#include "pch.hpp"

#include <sdl/sdl.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

namespace sdl {
    int SetRelativeMouseMode(int enabled) {
        static const auto fn = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_SetRelativeMouseMode");
        return fn.Call<decltype(&SetRelativeMouseMode)>(enabled);
    }

    int SetWindowGrab(void* window, int grabbed) {
        static const auto fn = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_SetWindowGrab");
        return fn.Call<decltype(&SetWindowGrab)>(window, grabbed);
    }

    void WarpMouseInWindow(void* window, float x, float y) {
        static const auto fn = CMemory::GetProcAddress(CConstants::SDL_LIB, "SDL_WarpMouseInWindow");
        return fn.Call<decltype(&WarpMouseInWindow)>(window, x, y);
    }
}  // namespace sdl
