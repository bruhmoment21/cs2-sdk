#include <vector>

#include "skin_changer.hpp"

#include "../../sdk/interfaces/interfaces.hpp"

static std::vector<CEconItem*> g_vecAddedItems;

void skin_changer::Run() {
    if (!interfaces::pEngine->IsInGame()) return;

    CCSPlayerInventory* pInventory = CCSPlayerInventory::GetInstance();
    if (!pInventory) return;

    CCSPlayerController* pLocalPlayerController =
        interfaces::pEntitySystem->GetLocalPlayerController();
    if (!pLocalPlayerController) return;

    C_CSPlayerPawn* pLocalPawn =
        pLocalPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!pLocalPawn) return;

    CPlayer_WeaponServices* pWeaponServices = pLocalPawn->m_pWeaponServices();
    if (!pWeaponServices) return;

    CHandle hActiveWeapon = pWeaponServices->m_hActiveWeapon();

    CCSPlayer_ViewModelServices* pViewmodelServices =
        pLocalPawn->m_pViewModelServices();
    if (!pViewmodelServices) return;

    C_BaseEntity* pViewmodel = pViewmodelServices->m_hViewModel()[0].Get();
    if (!pViewmodel) return;

    CGameSceneNode* pViewmodelSceneNode = pViewmodel->m_pGameSceneNode();
    if (!pViewmodelSceneNode) return;

    CNetworkUtlVectorBase<CHandle>* pWeapons = pWeaponServices->m_hMyWeapons();
    if (!pWeapons) return;

    for (int i = 0; i < pWeapons->m_size; ++i) {
        CHandle hWeapon = pWeapons->m_data[i];
        C_WeaponCSBase* pWeapon = hWeapon.Get<C_WeaponCSBase>();
        if (!pWeapon || pWeapon->GetOriginalOwnerXuid() !=
                            pLocalPlayerController->m_steamID())
            continue;

        C_AttributeContainer* pAttributeContainer =
            pWeapon->m_AttributeManager();
        if (!pAttributeContainer) continue;

        C_EconItemView* pWeaponItemView = pAttributeContainer->m_Item();
        if (!pWeaponItemView) continue;

        CEconItemDefinition* pWeaponDefinition =
            pWeaponItemView->GetStaticData();
        if (!pWeaponDefinition) continue;

        CGameSceneNode* pWeaponSceneNode = pWeapon->m_pGameSceneNode();
        if (!pWeaponSceneNode) continue;

        C_EconItemView* pWeaponInLoadoutItemView =
            pInventory->GetItemInLoadout(pWeapon->m_iOriginalTeamNumber(),
                                         pWeaponDefinition->GetLoadoutSlot());
        if (!pWeaponInLoadoutItemView) continue;

        // Check if skin is added by us.
        auto it = std::find_if(g_vecAddedItems.cbegin(), g_vecAddedItems.cend(),
                               [pWeaponInLoadoutItemView](CEconItem* i) {
                                   return i->m_ulID ==
                                          pWeaponInLoadoutItemView->m_iItemID();
                               });
        if (it == g_vecAddedItems.cend()) continue;

        pWeaponItemView->m_iItemID() = pWeaponInLoadoutItemView->m_iItemID();
        pWeaponItemView->m_iItemIDHigh() =
            pWeaponInLoadoutItemView->m_iItemIDHigh();
        pWeaponItemView->m_iItemIDLow() =
            pWeaponInLoadoutItemView->m_iItemIDLow();
        pWeaponItemView->m_iAccountID() =
            uint32_t(pInventory->GetOwnerID().m_id);

        // Workaround: We are forcing the OLD Models.
        pWeaponSceneNode->SetMeshGroupMask(2);
        if (hWeapon == hActiveWeapon) pViewmodelSceneNode->SetMeshGroupMask(2);
    }
}

void skin_changer::AddEconItemToList(CEconItem* pItem) {
    g_vecAddedItems.emplace_back(pItem);
}

void skin_changer::Shutdown() {
    CCSPlayerInventory* pInventory = CCSPlayerInventory::GetInstance();
    if (!pInventory) return;

    for (CEconItem* pItem : g_vecAddedItems) pInventory->RemoveEconItem(pItem);
}
