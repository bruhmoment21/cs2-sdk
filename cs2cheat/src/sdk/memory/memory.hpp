#pragma once

#include "../../api/module/module.hpp"

class Vector;

namespace memory {
    void Initialize();
    void Shutdown();

    inline void*(__thiscall* fnGetClientNetworkable)(void*, int);
    inline int(__thiscall* fnGetMaxEntities)(void*, int*);
    inline void*(__thiscall* schema_detailed_class_layout)(void*, const char*);
    inline bool(__thiscall* fnScreenTransform)(const Vector&, Vector&);
}  // namespace memory
