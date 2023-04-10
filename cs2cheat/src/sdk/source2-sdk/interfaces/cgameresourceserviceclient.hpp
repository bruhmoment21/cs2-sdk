#pragma once

#include "../../virtual.hpp"

class CGameEntitySystem;

class CGameResourceService {
   public:
    CGameEntitySystem* GetGameEntitySystem() {
        return *reinterpret_cast<CGameEntitySystem**>((uintptr_t)(this) + 0x58);
    }
};
