#include <unordered_map>

#include "skin_changer.hpp"

#include "../../sdk/interfaces/interfaces.hpp"

// More details:
// https://www.unknowncheats.me/forum/3715213-post352.html

// I know that skins are being applied wrong.
// Cause: https://www.unknowncheats.me/forum/3715116-post351.html

static const std::unordered_map<uint16_t, int32_t> g_skinsMap{
    {4, 38},    // Glock 18 | Fade
    {7, 302},   // AK47 | Vulcan
    {32, 260},  // P2000 | Ivory
    {61, 504},  // USP | Kill Confirmed
};

void skin_changer::Run() {
    // https://www.unknowncheats.me/forum/3711351-post132.html
    if (!interfaces::pEngine->IsInGame()) return;

    CCSPlayerController* pLocalPlayerController =
        interfaces::pEntitySystem->GetLocalPlayerController();
    if (!pLocalPlayerController) return;

    C_CSPlayerPawn* pLocalPawn =
        pLocalPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!pLocalPawn) return;

    CPlayer_WeaponServices* pWeaponServices = pLocalPawn->m_pWeaponServices();
    if (!pWeaponServices) return;

    CNetworkUtlVectorBase<CHandle>* pWeapons = pWeaponServices->m_hMyWeapons();
    if (!pWeapons) return;

    for (size_t i = 0; i < pWeapons->size; ++i) {
        C_BasePlayerWeapon* pWeapon =
            pWeapons->pData[i].Get<C_BasePlayerWeapon>();
        if (!pWeapon) continue;

        C_AttributeContainer* pAttributeContainer =
            pWeapon->m_AttributeManager();
        if (!pAttributeContainer) continue;

        C_EconItemView* pEconItemView = pAttributeContainer->m_Item();
        if (!pEconItemView) continue;

        const auto& skinEntry =
            g_skinsMap.find(pEconItemView->m_iItemDefinitionIndex());
        if (skinEntry == g_skinsMap.cend()) continue;

        pEconItemView->m_iItemIDLow() = -1;

        pWeapon->m_flFallbackWear() = 0.0001f;
        pWeapon->m_nFallbackSeed() = 0;
        pWeapon->m_nFallbackPaintKit() = skinEntry->second;
    }
}
