#pragma once

#include "../../../memory/memory.hpp"
#include "../../../virtual.hpp"

#include "../entity/ccsplayercontroller.hpp"

class CGameEntitySystem {
   public:
    template <typename T = C_BaseEntity>
    T* GetBaseEntity(int index) {
        if (memory::fnGetBaseEntity)
            return (T*)(memory::fnGetBaseEntity(this, index));
        return nullptr;
    }

    int GetHighestEntityIndex() {
        int highestIdx = -1;
        if (memory::fnGetHighestEntityIndex)
            memory::fnGetHighestEntityIndex(this, &highestIdx);
        return highestIdx;
    }

    CCSPlayerController* GetLocalPlayerController() {
        if (memory::fnGetLocalPlayerController)
            return memory::fnGetLocalPlayerController(-1);
        return nullptr;
    }
};
