#pragma once

#include "../../../virtual.hpp"

#include "../entity/ccsplayercontroller.hpp"

class CGameEntitySystem {
   public:
    template <typename T = C_BaseEntity>
    T* GetBaseEntity(CEntityIndex index) {
        if (memory::fnGetBaseEntity)
            return (T*)(memory::fnGetBaseEntity(this, index));
        return nullptr;
    }

    template <typename T = C_BaseEntity>
    T* GetBaseEntityFromHandle(CEntityHandle handle) {
        if (memory::fnGetBaseEntityFromHandle)
            return (T*)(memory::fnGetBaseEntityFromHandle(this, handle));
        return nullptr;
    }

    int GetHighestEntityIndex() {
        int highestIdx = -1;
        if (memory::fnGetHighestEntityIndex)
            memory::fnGetHighestEntityIndex(this, &highestIdx);
        return highestIdx;
    }

    CCSPlayerController* GetLocalPlayerController() {
        // Don't use this inside a loop.
        for (int i = 1; i <= MAX_PLAYERS; ++i) {
            CCSPlayerController* pPlayerController =
                GetBaseEntity<CCSPlayerController>(i);
            if (!pPlayerController || !pPlayerController->IsPlayerController())
                continue;

            if (pPlayerController->m_bIsLocalPlayerController())
                return pPlayerController;
        }

        return nullptr;
    }
};
