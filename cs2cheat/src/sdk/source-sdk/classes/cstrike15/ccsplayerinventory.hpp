#pragma once

#include <utility>

#include "../gcsdk/cgcclientsystem.hpp"
#include "../entity/c_econitemview.hpp"
#include "../entity/ceconitem.hpp"
#include "../types/utlvector.hpp"

class CCSPlayerInventory {
   public:
    static CCSPlayerInventory* GetInstance();

    bool AddEconItem(CEconItem* pItem, bool bUpdateAckFile, bool bWriteAckFile,
                     bool bCheckForNewItems);
    std::pair<uint64_t, uint32_t> GetHighestIDs();

    auto GetOwnerID() {
        return *reinterpret_cast<SOID_t*>((uintptr_t)(this) + 0x10);
    }

    auto& GetVecInventoryItems() {
        return *reinterpret_cast<CUtlVector<C_EconItemView*>*>(
            (uintptr_t)(this) + 0x38);
    }
};
