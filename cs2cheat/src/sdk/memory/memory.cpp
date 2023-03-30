#include "memory.hpp"

#include <stb/stb.hh>

#define LOG_RESULT(src) LOG("memory::%s found at -> %p\n", #src, src)

void memory::Initialize() {
    CModule client(CLIENT_DLL, true);
    CModule schemasystem(SCHEMASYSTEM_DLL, true);
    CModule sdl2(SDL2_DLL, true);

    fnGetClientNetworkable = client.FindPattern(GET_CLIENT_NETWORKABLE)
                                 .ToAbsolute(3, 0)
                                 .GetAs<decltype(fnGetClientNetworkable)>();
    LOG_RESULT(fnGetClientNetworkable);
    fnGetMaxEntities = client.FindPattern(GET_MAX_ENTITIES)
                           .ToAbsolute(3, 0)
                           .GetAs<decltype(fnGetMaxEntities)>();
    LOG_RESULT(fnGetMaxEntities);
    schema_detailed_class_layout =
        schemasystem.FindPattern(PRINT_SCHEMA_DETAILED_CLASS_LAYOUT)
            .GetAs<decltype(schema_detailed_class_layout)>();
    LOG_RESULT(schema_detailed_class_layout);
    fnScreenTransform = client.FindPattern(SCREEN_TRANSFORM)
                            .ToAbsolute(1, 0)
                            .GetAs<decltype(fnScreenTransform)>();
    LOG_RESULT(fnScreenTransform);
    fnMouseInputEnabled =
        client.FindPattern(MOUSE_INPUT_ENABLED).GetAs<void*>();
    LOG_RESULT(fnMouseInputEnabled);

    // SDL Functions:
    fnSDL_SetRelativeMouseMode =
        sdl2.GetProcAddress<decltype(fnSDL_SetRelativeMouseMode)>(
            "SDL_SetRelativeMouseMode");
    LOG_RESULT(fnSDL_SetRelativeMouseMode);
    fnSDL_SetWindowGrab =
        sdl2.GetProcAddress<decltype(fnSDL_SetWindowGrab)>("SDL_SetWindowGrab");
    LOG_RESULT(fnSDL_SetWindowGrab);
    fnSDL_WarpMouseInWindow =
        sdl2.GetProcAddress<decltype(fnSDL_WarpMouseInWindow)>(
            "SDL_WarpMouseInWindow");
    LOG_RESULT(fnSDL_WarpMouseInWindow);
}

void memory::Shutdown() {}
