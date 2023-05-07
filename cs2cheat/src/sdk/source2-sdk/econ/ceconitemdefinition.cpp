#include "ceconitemdefinition.hpp"

#include <fnv1a/hash_fnv1a_constexpr.h>

bool CEconItemDefinition::IsWeapon() {
    // Every gun supports at least 4 stickers.
    return GetStickersSupportedCount() >= 4;
}

bool CEconItemDefinition::IsKnife(bool excludeDefault) {
    static constexpr auto CSGO_Type_Knife =
        hash_32_fnv1a_const("#CSGO_Type_Knife");

    if (hash_32_fnv1a_const(m_pszItemTypeName) != CSGO_Type_Knife) return false;
    return excludeDefault ? m_nDefIndex >= 500 : true;
}

bool CEconItemDefinition::IsGlove(bool excludeDefault) {
    static constexpr auto Type_Hands = hash_32_fnv1a_const("#Type_Hands");

    if (hash_32_fnv1a_const(m_pszItemTypeName) != Type_Hands) return false;
    const bool defaultGlove = m_nDefIndex == 5028 || m_nDefIndex == 5029;

    return excludeDefault ? !defaultGlove : true;
}
