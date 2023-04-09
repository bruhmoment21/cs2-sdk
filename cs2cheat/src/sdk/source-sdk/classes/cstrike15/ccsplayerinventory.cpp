#include "ccsplayerinventory.hpp"

#include "../../../memory/memory.hpp"

#include "../gcsdk/cgcclientsharedobjecttypecache.hpp"

#include "ccsinventorymanager.hpp"

static CGCClientSharedObjectTypeCache* CreateBaseTypeCache(
    CCSPlayerInventory* pInventory) {
    CGCClientSystem* pGCClientSystem = CGCClientSystem::GetInstance();
    if (!pGCClientSystem) return nullptr;

    CGCClient* pGCClient = pGCClientSystem->GetCGCClient();
    if (!pGCClient) return nullptr;

    CGCClientSharedObjectCache* pSOCache =
        pGCClient->FindSOCache(pInventory->GetOwnerID());
    if (!pSOCache) return nullptr;

    return pSOCache->CreateBaseTypeCache(1);
}

CCSPlayerInventory* CCSPlayerInventory::GetInstance() {
    if (memory::fnGetInventoryManager) {
        CCSInventoryManager* pInventoryManager =
            memory::fnGetInventoryManager();
        if (!pInventoryManager) return nullptr;

        return pInventoryManager->GetLocalInventory();
    }

    return nullptr;
}

void CCSPlayerInventory::AddEconItem(CEconItem* pItem) {
    // Helper function to aid in adding items.
    if (!pItem) return;

    CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
    if (!pSOTypeCache || !pSOTypeCache->AddObject(pItem)) return;

    SOCreated(GetOwnerID(), pItem, eSOCacheEvent_Incremental);
}

void CCSPlayerInventory::RemoveEconItem(CEconItem* pItem) {
    // Helper function to aid in removing items.
    if (!pItem) return;

    SODestroyed(GetOwnerID(), pItem, eSOCacheEvent_Incremental);

    CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
    if (pSOTypeCache) pSOTypeCache->RemoveObject(pItem);

    pItem->Destruct();
}

std::pair<uint64_t, uint32_t> CCSPlayerInventory::GetHighestIDs() {
    uint64_t maxItemID = 0;
    uint32_t maxInventoryID = 0;

    CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
    if (pSOTypeCache) {
        const CUtlVector<CEconItem*>& vecItems =
            pSOTypeCache->GetVecObjects<CEconItem*>();

        for (int i = 0; i < vecItems.m_size; ++i) {
            CEconItem* pEconItem = vecItems.m_data[i];

            // Checks if item is default.
            if ((pEconItem->m_ulID & 0xF000000000000000) != 0) continue;

            maxItemID = std::max(maxItemID, pEconItem->m_ulID);
            maxInventoryID = std::max(maxInventoryID, pEconItem->m_unInventory);
        }
    }

    return std::make_pair(maxItemID, maxInventoryID);
}
