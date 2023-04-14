#include "memory.hpp"

#include <stb/stb.hh>

#define LOG_RESULT(src) LOG("memory::%s found at -> %p\n", #src, src)

void memory::Initialize() {
    CModule client(CLIENT_DLL);
    CModule schemasystem(SCHEMASYSTEM_DLL);
    CModule sdl2(SDL2_DLL);
    CModule tier0(TIER0_DLL);

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
    fnMouseInputEnabled = client.FindPattern(MOUSE_INPUT_ENABLED).Get<void*>();
    LOG_RESULT(fnMouseInputEnabled);
    fnSetMeshGroupMask = client.FindPattern(SET_MESH_GROUP_MASK)
                             .ToAbsolute(1, 0)
                             .Get<decltype(fnSetMeshGroupMask)>();
    LOG_RESULT(fnSetMeshGroupMask);
    fnGetInventoryManager = client.FindPattern(GET_INVENTORY_MANAGER)
                                .ToAbsolute(1, 0)
                                .Get<decltype(fnGetInventoryManager)>();
    LOG_RESULT(fnGetInventoryManager);
    fnGetClientSystem = client.FindPattern(GET_GC_CLIENT_SYSTEM)
                            .ToAbsolute(1, 0)
                            .Get<decltype(fnGetClientSystem)>();
    LOG_RESULT(fnGetClientSystem);
    fnCreateSharedObjectSubclassEconItem =
        client.FindPattern(CREATE_SHARED_OBJECT_SUBCLASS_ECON_ITEM)
            .Get<decltype(fnCreateSharedObjectSubclassEconItem)>();
    LOG_RESULT(fnCreateSharedObjectSubclassEconItem);
    fnCreateBaseTypeCache = client.FindPattern(CREATE_BASE_TYPE_CACHE)
                                .ToAbsolute(1, 0)
                                .Get<decltype(fnCreateBaseTypeCache)>();
    LOG_RESULT(fnCreateBaseTypeCache);
    fnFindSOCache = client.FindPattern(FIND_SO_CACHE)
                        .ToAbsolute(1, 0)
                        .Get<decltype(fnFindSOCache)>();
    LOG_RESULT(fnFindSOCache);
    fnGetLocalPlayerController =
        client.FindPattern(GET_LOCAL_PLAYER_CONTROLLER)
            .ToAbsolute(1, 0)
            .Get<decltype(fnGetLocalPlayerController)>();
    LOG_RESULT(fnGetLocalPlayerController);
    fnSetDynamicAttributeValueFloat =
        client.FindPattern(SET_DYNAMIC_ATTRIBUTE_VALUE_FLOAT)
            .ToAbsolute(1, 0)
            .Get<decltype(fnSetDynamicAttributeValueFloat)>();
    LOG_RESULT(fnSetDynamicAttributeValueFloat);
    fnSetCustomNameOrDescAttribute =
        client.FindPattern(SET_CUSTOM_NAME_OR_DESC_ATTRIBUTE)
            .ToAbsolute(1, 0)
            .Get<decltype(fnSetCustomNameOrDescAttribute)>();
    LOG_RESULT(fnSetCustomNameOrDescAttribute);
    fnSetModel = client.FindPattern(SET_MODEL)
                     .ToAbsolute(1, 0)
                     .Get<decltype(fnSetModel)>();
    LOG_RESULT(fnSetModel);
    fnComputeHitboxSurroundingBox =
        client.FindPattern(COMPUTE_HITBOX_SURROUNDING_BOX)
            .ToAbsolute(1, 0)
            .Get<decltype(fnComputeHitboxSurroundingBox)>();
    LOG_RESULT(fnComputeHitboxSurroundingBox);
    fnGetMatricesForView = client.FindPattern(GET_MATRICES_FOR_VIEW)
                               .Get<decltype(fnGetMatricesForView)>();
    LOG_RESULT(fnGetMatricesForView);

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

    auto ppHeapMemAlloc = tier0.GetProcAddress<CHeapMemAlloc**>("g_pMemAlloc");
    if (ppHeapMemAlloc) {
        s_HeapMemAlloc = *ppHeapMemAlloc;
        LOG_RESULT(s_HeapMemAlloc);
    }
}

void memory::Shutdown() {}
