#pragma once

#include "../gcsdk/csharedobject.hpp"

class CEconItem {
    void SetDynamicAttributeValueFloat(int index, float value);
    void SetDynamicAttributeValueString(int index, const char* value);

   public:
    static CEconItem* CreateInstance();

    auto Destruct() { return CALL_VIRTUAL(void, 1, this, true); }

    void SetPaintKit(int kit) { SetDynamicAttributeValueFloat(6, float(kit)); }
    void SetPaintSeed(int seed) {
        SetDynamicAttributeValueFloat(7, float(seed));
    }
    void SetPaintWear(float wear) { SetDynamicAttributeValueFloat(8, wear); }
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

class CEconDefaultEquippedDefinitionInstanceClient : public CSharedObject {
   public:
    auto& GetDefinitionIndex() {
        return *reinterpret_cast<short*>((uintptr_t)(this) + 0x24);
    }
};
