#pragma once

#include "../../../virtual.hpp"

#include "../types/utlvector.hpp"

class CEconItemDefinition {
   public:
    auto GetDefinitionIndex() { return CALL_VIRTUAL(uint16_t, 0, this); }

    auto GetStickersSupportedCount() {
        return *reinterpret_cast<int*>((uintptr_t)(this) + 0x100);
    }

    auto GetLoadoutSlot() {
        return *reinterpret_cast<int*>((uintptr_t)(this) + 0x2E8);
    }

    char pad0[0x8];  // vtable
    void* m_pKVItem;
    uint16_t m_nDefIndex;
    CUtlVector<uint16_t> m_nAssociatedItemsDefIndexes;
    bool m_bEnabled;
    const char* m_szPrefab;
    uint8_t m_unMinItemLevel;
    uint8_t m_unMaxItemLevel;
    uint8_t m_nItemRarity;
    uint8_t m_nItemQuality;
    uint8_t m_nForcedItemQuality;
    uint8_t m_nDefaultDropItemQuality;
    uint8_t m_nDefaultDropQuantity;
    CUtlVector<void*> m_vecStaticAttributes;
    uint8_t m_nPopularitySeed;
    void* m_pPortraitsKV;
    const char* m_pszItemBaseName;
    bool m_bProperName;
    const char* m_pszItemTypeName;
    uint32_t m_unItemTypeID;
    const char* m_pszItemDesc;
};
