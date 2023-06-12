#pragma once

#include "../../virtual.hpp"

class CSharedObject;

class CEconItem {
    void SetDynamicAttributeValue(int index, void* value);
    void SetDynamicAttributeValueString(int index, const char* value);

   public:
    static CEconItem* CreateInstance();

    auto Destruct() { return CALL_VIRTUAL(void, 1, this, true); }

    void SetPaintKit(float kit) { SetDynamicAttributeValue(6, &kit); }
    void SetPaintSeed(float seed) { SetDynamicAttributeValue(7, &seed); }
    void SetPaintWear(float wear) { SetDynamicAttributeValue(8, &wear); }
    void SetStatTrak(int count) { SetDynamicAttributeValue(80, &count); }
    void SetStatTrakType(int type) { SetDynamicAttributeValue(81, &type); }
    void SetCustomName(const char* pName) {
        SetDynamicAttributeValueString(111, pName);
    }

    char pad0[0x10];  // 2 vtables
    uint64_t m_ulID;
    uint64_t m_ulOriginalID;
    void* m_pCustomDataOptimizedObject;
    uint32_t m_unAccountID;
    uint32_t m_unInventory;
    uint16_t m_unDefIndex;
    uint16_t m_unOrigin : 5;
    uint16_t m_nQuality : 4;
    uint16_t m_unLevel : 2;
    uint16_t m_nRarity : 4;
    uint16_t m_dirtybitInUse : 1;
    int16_t m_iItemSet;
    int m_bSOUpdateFrame;
    uint8_t m_unFlags;
};
