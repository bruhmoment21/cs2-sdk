#pragma once

#include <utility>

#include "../gcsdk/cgcclientsharedobjectcache.hpp"
#include "../gcsdk/cgcclientsystem.hpp"
#include "../entity/c_econitemview.hpp"
#include "../types/utlvector.hpp"
#include "../econ/ceconitem.hpp"

#include "ccsinventorymanager.hpp"

class CCSPlayerInventory {
   public:
    static CCSPlayerInventory* GetInstance();

    auto SOCreated(SOID_t owner, CSharedObject* pObject, ESOCacheEvent eEvent) {
        return CALL_VIRTUAL(void, 0, this, owner, pObject, eEvent);
    }

    auto SOUpdated(SOID_t owner, CSharedObject* pObject, ESOCacheEvent eEvent) {
        return CALL_VIRTUAL(void, 1, this, owner, pObject, eEvent);
    }

    auto SODestroyed(SOID_t owner, CSharedObject* pObject,
                     ESOCacheEvent eEvent) {
        return CALL_VIRTUAL(void, 2, this, owner, pObject, eEvent);
    }

    auto GetItemInLoadout(int iClass, int iSlot) {
        return CALL_VIRTUAL(C_EconItemView*, 8, this, iClass, iSlot);
    }

    bool AddEconItem(CEconItem* pItem);
    void RemoveEconItem(CEconItem* pItem);
    std::pair<uint64_t, uint32_t> GetHighestIDs();
    C_EconItemView* GetItemViewForItem(uint64_t itemID);
    CEconItem* GetSOCDataForItem(uint64_t itemID);

    auto GetOwner() {
        return *reinterpret_cast<SOID_t*>((uintptr_t)(this) + 0x10);
    }

    auto& GetItemVector() {
        return *reinterpret_cast<CUtlVector<C_EconItemView*>*>(
            (uintptr_t)(this) + 0x20);
    }
};
