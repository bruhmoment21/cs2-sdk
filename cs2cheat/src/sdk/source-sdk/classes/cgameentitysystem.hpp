#pragma once

#include "../../virtual.hpp"

#include "entity/ccsplayercontroller.hpp"

class CGameEntitySystem {
   public:
    template <typename T = C_BaseEntity>
    T* GetBaseEntity(int index) {
        if (memory::fnGetClientNetworkable)
            return (T*)(memory::fnGetClientNetworkable(this, index));
        return nullptr;
    }

    int GetMaxEntities() {
        int highestIdx = -1;
        if (memory::fnGetMaxEntities)
            memory::fnGetMaxEntities(this, &highestIdx);
        return highestIdx;
    }
};
