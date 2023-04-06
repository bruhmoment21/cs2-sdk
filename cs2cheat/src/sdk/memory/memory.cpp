#include "memory.hpp"

#include <stb/stb.hh>

#define LOG_RESULT(src) LOG("memory::%s found at -> %p\n", #src, src)

void memory::Initialize() {
    CModule client(CLIENT_DLL, true);
    CModule schemasystem(SCHEMASYSTEM_DLL, true);
    CModule sdl2(SDL2_DLL, true);

    fnGetBaseEntity = client.FindPattern(GET_BASE_ENTITY)
                          .ToAbsolute(3, 0)
                          .Get<decltype(fnGetBaseEntity)>();
    LOG_RESULT(fnGetBaseEntity);
    fnGetHighestEntityIndex = client.FindPattern(GET_HIGHEST_ENTITY_INDEX)
                                  .ToAbsolute(3, 0)
                                  .Get<decltype(fnGetHighestEntityIndex)>();
    LOG_RESULT(fnGetHighestEntityIndex);
    schema_detailed_class_layout =
        schemasystem.FindPattern(PRINT_SCHEMA_DETAILED_CLASS_LAYOUT)
            .Get<decltype(schema_detailed_class_layout)>();
    LOG_RESULT(schema_detailed_class_layout);
    fnScreenTransform = client.FindPattern(SCREEN_TRANSFORM)
                            .ToAbsolute(1, 0)
                            .Get<decltype(fnScreenTransform)>();
    LOG_RESULT(fnScreenTransform);
    fnMouseInputEnabled = client.FindPattern(MOUSE_INPUT_ENABLED).Get<void*>();
    LOG_RESULT(fnMouseInputEnabled);
    fnSetMeshGroupMask = client.FindPattern(SET_MESH_GROUP_MASK)
                             .ToAbsolute(1, 0)
                             .Get<decltype(fnSetMeshGroupMask)>();
    LOG_RESULT(fnSetMeshGroupMask);

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
