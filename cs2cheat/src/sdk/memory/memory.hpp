#pragma once

#include "../../api/module/module.hpp"

#include "../source-sdk/classes/cstrike15/ccsplayerinventory.hpp"

class Vector;
class CCSInventoryManager;
class CGCClientSharedObjectTypeCache;

namespace memory {
    void Initialize();
    void Shutdown();

    inline void*(__thiscall* fnGetBaseEntity)(void*, int);
    inline int(__thiscall* fnGetHighestEntityIndex)(void*, int*);
    inline void*(__thiscall* schema_detailed_class_layout)(void*, const char*);
    inline bool(__thiscall* fnScreenTransform)(const Vector&, Vector&);
    inline void* fnMouseInputEnabled;
    inline void(__thiscall* fnSetMeshGroupMask)(void*, uint64_t);
    inline CCSInventoryManager* (*fnGetInventoryManager)();
    inline CGCClientSystem* (*fnGetClientSystem)();
    inline bool(__thiscall* fnAddEconItem)(void*, CEconItem*, bool, bool, bool);
    inline CEconItem* (*fnCreateSharedObjectSubclassEconItem)();
    inline CGCClientSharedObjectTypeCache*(__thiscall* fnCreateBaseTypeCache)(
        void*, int);
    inline CGCClientSharedObjectCache*(__thiscall* fnFindSOCache)(void*, SOID_t,
                                                                  bool);

    // SDL2 Functions
    inline int(__stdcall* fnSDL_SetRelativeMouseMode)(int);
    inline int(__stdcall* fnSDL_SetWindowGrab)(void*, int);
    inline int(__stdcall* fnSDL_WarpMouseInWindow)(void*, int, int);
}  // namespace memory
