#pragma once

#include "../../api/module/module.hpp"

#include "../source2-sdk/cstrike15/ccsplayerinventory.hpp"

class Vector;
class CCSInventoryManager;
class CCSPlayerController;

namespace memory {
    void Initialize();
    void Shutdown();

    inline void*(__thiscall* fnGetBaseEntity)(void*, int);
    inline int(__thiscall* fnGetHighestEntityIndex)(void*, int*);
    inline void*(__thiscall* schema_detailed_class_layout)(void*, const char*);
    inline void* fnMouseInputEnabled;
    inline void(__thiscall* fnSetMeshGroupMask)(void*, uint64_t);
    inline CCSInventoryManager* (*fnGetInventoryManager)();
    inline CGCClientSystem* (*fnGetClientSystem)();
    inline CEconItem* (*fnCreateSharedObjectSubclassEconItem)();
    inline CGCClientSharedObjectTypeCache*(__thiscall* fnCreateBaseTypeCache)(
        void*, int);
    inline CGCClientSharedObjectCache*(__thiscall* fnFindSOCache)(void*, SOID_t,
                                                                  bool);
    inline CCSPlayerController*(__fastcall* fnGetLocalPlayerController)(int);
    inline void*(__fastcall* fnSetDynamicAttributeValueUint)(void*, void*,
                                                             void*);
    inline void*(__fastcall* fnSetDynamicAttributeValueString)(void*, void*,
                                                               uint8_t*);
    inline void*(__fastcall* fnCAttributeStringInit)(void*, uint64_t, bool);
    inline void*(__fastcall* fnCAttributeStringFill)(void*, const char*);
    inline void*(__fastcall* fnSetModel)(void*, const char*);
    inline bool(__fastcall* fnComputeHitboxSurroundingBox)(void*, Vector&,
                                                           Vector&);
    inline void* fnGetMatricesForView;
    inline void* fnFireEventClientSide;
    inline void(__fastcall* fnAddStattrakEntity)(void*);
    inline void(__fastcall* fnAddNametagEntity)(void*);
    inline bool(__fastcall* fnIsPaintKitUsingLegacyModel)(const char*);

    // SDL2 Functions
    inline int(__stdcall* fnSDL_SetRelativeMouseMode)(int);
    inline int(__stdcall* fnSDL_SetWindowGrab)(void*, int);
    inline int(__stdcall* fnSDL_WarpMouseInWindow)(void*, float, float);
}  // namespace memory
