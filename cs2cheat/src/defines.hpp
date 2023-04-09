#pragma once

#include "sdk/game_defines.hpp"

// Uncomment this for vulkan support, add '-vulkan' into your launch
// parameters and lastly include both 'imgui_impl_vulkan' files.
// #define CS2_SDK_ENABLE_VULKAN_SUPPORT

// Helper
#define SIGNATURE(sig) \
    stb::simple_conversion::build<stb::fixed_string{sig}>::value

// Modules
#define CLIENT_DLL "client.dll"
#define ENGINE2_DLL "engine2.dll"
#define SCHEMASYSTEM_DLL "schemasystem.dll"
#define INPUTSYSTEM_DLL "inputsystem.dll"
#define SDL2_DLL "SDL2.dll"
#define TIER0_DLL "tier0.dll"
#define NAVSYSTEM_DLL "navsystem.dll"
#define RENDERSYSTEMVULKAN_DLL "rendersystemvulkan.dll"
#define LOCALIZE_DLL "localize.dll"

// Interfaces
#define GAME_RESOURCE_SERVICE_CLIENT "GameResourceServiceClientV001"
#define SOURCE2_CLIENT "Source2Client002"
#define SCHEMA_SYSTEM "SchemaSystem_001"
#define INPUT_SYSTEM_VERSION "InputSystemVersion001"
#define SOURCE2_ENGINE_TO_CLIENT "Source2EngineToClient001"
#define ENGINE_CVAR "VEngineCvar007"
#define LOCALIZE "Localize_001"

// Signatures

// CGameEntitySystem::
#define GET_HIGHEST_ENTITY_INDEX SIGNATURE("33 DB E8 ? ? ? ? 8B 08")
#define GET_BASE_ENTITY SIGNATURE("8B D3 E8 ? ? ? ? 48 8B F8 48 85 C0 74 76")

#define PRINT_SCHEMA_DETAILED_CLASS_LAYOUT \
    SIGNATURE("48 89 5C 24 ? 48 89 6C 24 ? 48 89 4C 24 ?")

#define SCREEN_TRANSFORM SIGNATURE("E8 ? ? ? ? F3 0F 10 45 ? 8B D0")

#define SET_MESH_GROUP_MASK SIGNATURE("E8 ? ? ? ? 8B 45 D0 48 8B 55 D8")
#define GET_ITEM_NAME SIGNATURE("48 89 5C 24 10 48 89 6C 24 18 48 89 7C 24 20 41 56 48 83 EC 40 48 83")
#define GET_INVENTORY_MANAGER SIGNATURE("E8 ? ? ? ? 8B D5 48 8B D8")
#define CREATE_SHARED_OBJECT_SUBCLASS_ECON_ITEM \
    SIGNATURE(                                  \
        "48 83 EC 28 B9 ? ? ? ? E8 ? ? ? ? 48 85 C0 74 3D 48 8D 0D ? ? ? ?")
#define GET_GC_CLIENT_SYSTEM SIGNATURE("E8 ? ? ? ? 8B 53 28 48 8B C8")
#define CREATE_BASE_TYPE_CACHE SIGNATURE("E8 ? ? ? ? 45 33 ED 41 8B D5")
#define FIND_SO_CACHE SIGNATURE("E8 ? ? ? ? 48 8B F0 48 85 C0 74 0E 4C 8B C3")
#define GET_LOCAL_PLAYER_CONTROLLER SIGNATURE("E8 ? ? ? ? 49 89 47 08")
#define SET_DYNAMIC_ATTRIBUTE_VALUE_FLOAT                                      \
    SIGNATURE(                                                                 \
        "E9 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 49 8B C0 48 " \
        "8B CA 48 8B D0")

// CCSGOInput::
#define MOUSE_INPUT_ENABLED \
    SIGNATURE("40 53 48 83 EC 20 80 B9 ? ? ? ? ? 48 8B D9 75 78")

// Internal

#include <cassert>
#define CS2_ASSERT(cond, ...) assert(cond)
