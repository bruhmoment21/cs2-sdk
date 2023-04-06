#include <unordered_map>

#include "skin_changer.hpp"

#include "../../sdk/interfaces/interfaces.hpp"

// Paint kits and weapon definition indexes:
// https://www.unknowncheats.me/forum/3715213-post352.html

struct SkinEntry_t {
    int32_t m_skinDefinitionIndex;
    bool m_isUsingOldModel;
};

static const std::unordered_map<uint16_t, SkinEntry_t> g_skinsMap{
    {4, {38, true}},    // Glock 18 | Fade
    {7, {302, true}},   // AK47 | Vulcan
    {32, {260, true}},  // P2000 | Ivory
    {61, {504, true}},  // USP | Kill Confirmed
    {9, {344, true}},   // AWP | Dragon Lore
    {1, {1090, true}},  // Desert Eagle | Ocean Drive
    {16, {309, true}},  // M4A4 | Howl
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

    CCSPlayer_ViewModelServices* pViewmodelServices =
        pLocalPawn->m_pViewModelServices();
    if (!pViewmodelServices) return;

    C_BaseEntity* pViewmodel = pViewmodelServices->m_hViewModel()[0].Get();
    if (!pViewmodel) return;

    CGameSceneNode* pViewmodelSceneNode = pViewmodel->m_pGameSceneNode();
    if (!pViewmodelSceneNode) return;

    CNetworkUtlVectorBase<CHandle>* pWeapons = pWeaponServices->m_hMyWeapons();
    if (!pWeapons) return;

    for (size_t i = 0; i < pWeapons->m_size; ++i) {
        C_BasePlayerWeapon* pWeapon =
            pWeapons->m_data[i].Get<C_BasePlayerWeapon>();
        if (!pWeapon) continue;

        C_AttributeContainer* pAttributeContainer =
            pWeapon->m_AttributeManager();
        if (!pAttributeContainer) continue;

        C_EconItemView* pEconItemView = pAttributeContainer->m_Item();
        if (!pEconItemView) continue;

        CGameSceneNode* pWeaponSceneNode = pWeapon->m_pGameSceneNode();
        if (!pWeaponSceneNode) continue;

        const auto& skinIt =
            g_skinsMap.find(pEconItemView->m_iItemDefinitionIndex());
        if (skinIt == g_skinsMap.cend()) continue;

        const SkinEntry_t& skinEntry = skinIt->second;
        const uint64_t skinMeshGroupMask =
            static_cast<uint64_t>(skinEntry.m_isUsingOldModel) + 1;

        pEconItemView->m_iItemIDLow() = pEconItemView->m_iItemIDHigh() = -1;

        pWeapon->m_flFallbackWear() = 0.0001f;
        pWeapon->m_nFallbackSeed() = 0;
        pWeapon->m_nFallbackPaintKit() = skinEntry.m_skinDefinitionIndex;

        pViewmodelSceneNode->SetMeshGroupMask(skinMeshGroupMask);
        pWeaponSceneNode->SetMeshGroupMask(skinMeshGroupMask);
    }
}
