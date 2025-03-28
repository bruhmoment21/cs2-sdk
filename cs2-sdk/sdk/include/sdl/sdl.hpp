#pragma once

namespace sdl {
    bool SetWindowRelativeMouseMode(void* window, int enabled);
    bool SetWindowMouseGrab(void* window, int grabbed);
    void WarpMouseInWindow(void* window, float x, float y);
}  // namespace sdl
