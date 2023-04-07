#pragma once

#include "../gcsdk/cgcclientsystem.hpp"
#include "../entity/ceconitem.hpp"

class CCSPlayerInventory {
   public:
    static CCSPlayerInventory* GetInstance();

    bool AddEconItem(CEconItem* pItem, bool bUpdateAckFile, bool bWriteAckFile,
                     bool bCheckForNewItems);

    uint32_t GetAccountID() {
        return *reinterpret_cast<uint32_t*>((uintptr_t)(this) + 0x10);
    }

    SOID_t GetSOID() {
        return *reinterpret_cast<SOID_t*>((uintptr_t)(this) + 0x10);
    }
};
