#pragma once

#include "../../memory/memory.hpp"
#include "../../virtual.hpp"

#include "../entity/ccsplayercontroller.hpp"
#include "../entity/c_baseviewmodel.hpp"
#include "../entity/c_chicken.hpp"

class CGameEntitySystem {
   public:
    static CGameEntitySystem* GetInstance();
    static CCSPlayerController* GetLocalPlayerController();

    template <typename T = C_BaseEntity>
    T* GetBaseEntity(int index) {
        if (!memory::fnGetBaseEntity) return nullptr;
        return (T*)(memory::fnGetBaseEntity(this, index));
    }

    int GetHighestEntityIndex() {
        int highestIdx = -1;
        if (!memory::fnGetHighestEntityIndex) return highestIdx;

        memory::fnGetHighestEntityIndex(this, &highestIdx);
        return highestIdx;
    }
};
