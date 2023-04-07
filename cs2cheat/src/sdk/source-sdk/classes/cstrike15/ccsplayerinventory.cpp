#include "ccsplayerinventory.hpp"

#include "../../../memory/memory.hpp"

#include "../gcsdk/cgcclientsharedobjecttypecache.hpp"
#include "../gcsdk/cgcclientsharedobjectcache.hpp"

#include "ccsinventorymanager.hpp"

static CGCClientSharedObjectTypeCache* CreateBaseTypeCache(
    CCSPlayerInventory* pInventory) {
    CGCClientSystem* pGCClientSystem = CGCClientSystem::GetInstance();
    if (!pGCClientSystem) return nullptr;

    CGCClient* pGCClient = pGCClientSystem->GetCGCClient();
    if (!pGCClient) return nullptr;

    CGCClientSharedObjectCache* pSOCache =
        pGCClient->FindSOCache(pInventory->GetSOID());
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

bool CCSPlayerInventory::AddEconItem(CEconItem* pItem, bool bUpdateAckFile,
                                     bool bWriteAckFile,
                                     bool bCheckForNewItems) {
    if (!memory::fnAddEconItem) return false;

    CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
    if (!pSOTypeCache || !pSOTypeCache->AddObject(pItem)) return false;

    return memory::fnAddEconItem(this, pItem, bUpdateAckFile, bWriteAckFile,
                                 bCheckForNewItems);
}
