#pragma once

#include "../../virtual.hpp"

#include "../types/utlmap.hpp"

class CEconItemDefinition;

inline constexpr uint64_t Helper_GetAlternateIconKeyForWeaponPaintWearItem(
    uint16_t nDefIdx, uint32_t nPaintId, uint32_t nWear) {
    return (nDefIdx << 16) + (nPaintId << 2) + nWear;
}

struct AlternateIconData_t {
    const char* sSimpleName;
    const char* sLargeSimpleName;

   private:
    char pad0[0x8];  // no idea
    char pad1[0x8];  // no idea
    char pad2[0x8];  // no idea
    char pad3[0x8];  // no idea
};

class CPaintKit {
   public:
    bool UsesLegacyModel();

    int nID;
    const char* sName;
    const char* sDescriptionString;
    const char* sDescriptionTag;
    char pad0[0x8];  // no idea
    char pad1[0x8];  // no idea
    char pad2[0x8];  // no idea
    char pad3[0x8];  // no idea
    char pad4[0x4];  // no idea
    int nRarity;
};

class CEconItemSchema {
   public:
    auto GetAttributeDefinitionInterface(int iAttribIndex) {
        return CALL_VIRTUAL(void*, 27, this, iAttribIndex);
    }

    auto& GetSortedItemDefinitionMap() {
        return *reinterpret_cast<CUtlMap<int, CEconItemDefinition*>*>(
            (uintptr_t)(this) + 0x120);
    }

    auto& GetAlternateIconsMap() {
        return *reinterpret_cast<CUtlMap<uint64_t, AlternateIconData_t>*>(
            (uintptr_t)(this) + 0x270);
    }

    auto& GetPaintKits() {
        return *reinterpret_cast<CUtlMap<int, CPaintKit*>*>((uintptr_t)(this) +
                                                            0x2E8);
    }
};
