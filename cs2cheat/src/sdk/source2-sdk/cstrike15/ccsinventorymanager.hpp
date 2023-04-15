#pragma once

#include "../../virtual.hpp"

class CCSPlayerInventory;

class CCSInventoryManager {
   public:
    static CCSInventoryManager* GetInstance();

    auto EquipItemInLoadout(int iTeam, int iSlot, uint64_t iItemID,
                            bool bSwap = false) {
        return CALL_VIRTUAL(bool, 50, this, iTeam, iSlot, iItemID, bSwap);
    }

    auto GetLocalInventory() {
        return CALL_VIRTUAL(CCSPlayerInventory*, 53, this);
    }
};
