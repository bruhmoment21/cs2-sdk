#include "memory.hpp"

#include <stb/stb.hh>

#define MEMORY_VARIABLE(var) var, "memory::" #var

void memory::Initialize() {
    CModule client(CLIENT_DLL);
    CModule schemasystem(SCHEMASYSTEM_DLL);
    CModule sdl3(SDL3_DLL);
    CModule tier0(TIER0_DLL);

    client.FindPattern(GET_BASE_ENTITY)
        .ToAbsolute(3, 0)
        .Get(MEMORY_VARIABLE(fnGetBaseEntity));

    client.FindPattern(GET_HIGHEST_ENTITY_INDEX)
        .ToAbsolute(3, 0)
        .Get(MEMORY_VARIABLE(fnGetHighestEntityIndex));

    schemasystem.FindPattern(PRINT_SCHEMA_DETAILED_CLASS_LAYOUT)
        .Get(MEMORY_VARIABLE(schema_detailed_class_layout));

    client.FindPattern(MOUSE_INPUT_ENABLED)
        .Get(MEMORY_VARIABLE(fnMouseInputEnabled));

    client.FindPattern(SET_MESH_GROUP_MASK)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnSetMeshGroupMask));

    client.FindPattern(GET_INVENTORY_MANAGER)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnGetInventoryManager));

    client.FindPattern(GET_GC_CLIENT_SYSTEM)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnGetClientSystem));

    client.FindPattern(CREATE_SHARED_OBJECT_SUBCLASS_ECON_ITEM)
        .Get(MEMORY_VARIABLE(fnCreateSharedObjectSubclassEconItem));

    client.FindPattern(CREATE_BASE_TYPE_CACHE)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnCreateBaseTypeCache));

    client.FindPattern(FIND_SO_CACHE)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnFindSOCache));

    client.FindPattern(GET_LOCAL_PLAYER_CONTROLLER)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnGetLocalPlayerController));

    client.FindPattern(SET_DYNAMIC_ATTRIBUTE_VALUE_UINT)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnSetDynamicAttributeValueUint));

    client.FindPattern(SET_MODEL).ToAbsolute(1, 0).Get(
        MEMORY_VARIABLE(fnSetModel));

    client.FindPattern(COMPUTE_HITBOX_SURROUNDING_BOX)
        .ToAbsolute(1, 0)
        .Get(MEMORY_VARIABLE(fnComputeHitboxSurroundingBox));

    client.FindPattern(GET_MATRICES_FOR_VIEW)
        .Get(MEMORY_VARIABLE(fnGetMatricesForView));

    client.FindPattern(FIRE_EVENT_CLIENT_SIDE)
        .Get(MEMORY_VARIABLE(fnFireEventClientSide));

    client.FindPattern(ADD_STATTRAK_ENTITY)
        .Get(MEMORY_VARIABLE(fnAddStattrakEntity));

    client.FindPattern(ADD_NAMETAG_ENTITY)
        .Get(MEMORY_VARIABLE(fnAddNametagEntity));

    client.FindPattern(IS_PAINT_KIT_USING_LEGACY_MODEL)
        .Get(MEMORY_VARIABLE(fnIsPaintKitUsingLegacyModel));

    // SDL Functions:
    sdl3.GetProcAddress("SDL_SetRelativeMouseMode")
        .Get(fnSDL_SetRelativeMouseMode);
    sdl3.GetProcAddress("SDL_SetWindowGrab").Get(fnSDL_SetWindowGrab);
    sdl3.GetProcAddress("SDL_WarpMouseInWindow").Get(fnSDL_WarpMouseInWindow);

    UTILPtr ppHeapMemAlloc = tier0.GetProcAddress("g_pMemAlloc");
    if (ppHeapMemAlloc.IsValid())
        ppHeapMemAlloc.Dereference(1).Get(MEMORY_VARIABLE(g_pHeapMemAlloc));
}

void memory::Shutdown() {}
