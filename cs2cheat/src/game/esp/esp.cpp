#include <vector>
#include <mutex>

#include "esp.hpp"

#include "../../sdk/interfaces/interfaces.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

static ImDrawList* g_pBackgroundDrawList = nullptr;
static CCSPlayerController* g_pLocalPlayerController = nullptr;
static C_CSPlayerPawn* g_pLocalPlayerPawn = nullptr;

// Cache example:
struct CachedEntity_t {
    enum Type { UNKNOWN = 0, PLAYER_CONTROLLER, BASE_WEAPON, CHICKEN };

    CHandle m_handle;
    Type m_type;

    BBox_t m_bbox;
    bool m_draw;  // If player is not visible it will be false.
};
static std::vector<CachedEntity_t> g_cachedEntities;
static std::mutex g_cachedEntitiesMutex;

static CachedEntity_t::Type GetEntityType(C_BaseEntity* pEntity);
static void RenderPlayerESP(CCSPlayerController* pPlayerController,
                            const BBox_t& bBox);
static void RenderWeaponESP(C_CSWeaponBase* pWeapon, const BBox_t& bBox);
static void RenderWeaponName(C_CSWeaponBase* pWeapon, const BBox_t& bBox);
static void RenderChickenESP(C_Chicken* pChicken, const BBox_t& bBox);

void esp::Render() {
    if (!interfaces::pEngine->IsInGame()) return;

    g_pBackgroundDrawList = ImGui::GetBackgroundDrawList();

    g_pLocalPlayerController = CGameEntitySystem::GetLocalPlayerController();
    if (!g_pLocalPlayerController) return;

    g_pLocalPlayerPawn =
        g_pLocalPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!g_pLocalPlayerPawn) return;

    const std::lock_guard<std::mutex> guard{g_cachedEntitiesMutex};

    for (const auto& it : g_cachedEntities) {
        if (!it.m_draw) continue;

        C_BaseEntity* pEntity = it.m_handle.Get();
        if (!pEntity) continue;

        // Additional sanity check.
        CHandle hEntity = pEntity->GetRefEHandle();
        if (hEntity != it.m_handle) continue;

        switch (it.m_type) {
            case CachedEntity_t::PLAYER_CONTROLLER:
                RenderPlayerESP((CCSPlayerController*)pEntity, it.m_bbox);
                break;
            case CachedEntity_t::BASE_WEAPON:
                RenderWeaponESP((C_CSWeaponBase*)pEntity, it.m_bbox);
                break;
            case CachedEntity_t::CHICKEN:
                RenderChickenESP((C_Chicken*)pEntity, it.m_bbox);
                break;
        }
    }
}

void esp::CalculateBoundingBoxes() {
    if (!interfaces::pEngine->IsInGame()) return;

    const std::lock_guard<std::mutex> guard{g_cachedEntitiesMutex};

    for (auto& it : g_cachedEntities) {
        C_BaseEntity* pEntity = it.m_handle.Get();
        if (!pEntity) continue;

        // Additional sanity check.
        CHandle hEntity = pEntity->GetRefEHandle();
        if (hEntity != it.m_handle) continue;

        switch (it.m_type) {
            case CachedEntity_t::PLAYER_CONTROLLER: {
                C_CSPlayerPawn* pPlayerPawn = ((CCSPlayerController*)pEntity)
                                                  ->m_hPawn()
                                                  .Get<C_CSPlayerPawn>();
                if (pPlayerPawn)
                    it.m_draw = pPlayerPawn->GetBoundingBox(it.m_bbox, false);

            } break;
            case CachedEntity_t::BASE_WEAPON:
                it.m_draw = pEntity->GetBoundingBox(it.m_bbox, true);
                break;
            case CachedEntity_t::CHICKEN:
                it.m_draw = pEntity->GetBoundingBox(it.m_bbox, false);
                break;
        }
    }
}

void esp::CacheCurrentEntities() {
    // This function runs ONCE on injection and caches all the entities if
    // you happen to inject connected on a server.

    if (!interfaces::pEngine->IsInGame()) return;

    CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance();
    if (!pEntitySystem) return;

    int highestIndex = pEntitySystem->GetHighestEntityIndex();
    for (int i = 1; i <= highestIndex; ++i) {
        C_BaseEntity* pEntity = pEntitySystem->GetBaseEntity(i);
        if (!pEntity) continue;

        OnAddEntity(pEntity, pEntity->GetRefEHandle());
    }
}

void esp::OnAddEntity(CEntityInstance* pInst, CHandle handle) {
    C_BaseEntity* pEntity = (C_BaseEntity*)pInst;
    if (!pEntity) return;

    // Cache only networked entities.
    // https://developer.valvesoftware.com/wiki/Entity_limit#Source_2_limits
    if (handle.GetEntryIndex() >= 16384) return;

    auto it = std::find_if(g_cachedEntities.begin(), g_cachedEntities.end(),
                           [handle](const CachedEntity_t& i) {
                               return i.m_handle.GetEntryIndex() ==
                                      handle.GetEntryIndex();
                           });

    if (it == g_cachedEntities.end()) {
        CachedEntity_t cachedEntity{};
        cachedEntity.m_handle = handle;
        cachedEntity.m_type = ::GetEntityType(pEntity);
        if (cachedEntity.m_type != CachedEntity_t::UNKNOWN)
            g_cachedEntities.emplace_back(cachedEntity);
    } else {
        it->m_handle = handle;
        it->m_type = ::GetEntityType(pEntity);
    }
}

void esp::OnRemoveEntity(CEntityInstance* pInst, CHandle handle) {
    C_BaseEntity* pEntity = (C_BaseEntity*)pInst;
    if (!pEntity) return;

    auto it = std::find_if(
        g_cachedEntities.begin(), g_cachedEntities.end(),
        [handle](const CachedEntity_t& i) { return i.m_handle == handle; });
    if (it == g_cachedEntities.end()) return;

    it->m_draw = false;
    it->m_type = CachedEntity_t::UNKNOWN;
}

static CachedEntity_t::Type GetEntityType(C_BaseEntity* pEntity) {
    if (pEntity->IsBasePlayerController())
        return CachedEntity_t::PLAYER_CONTROLLER;
    else if (pEntity->IsBasePlayerWeapon())
        return CachedEntity_t::BASE_WEAPON;
    else if (pEntity->IsChicken())
        return CachedEntity_t::CHICKEN;

    return CachedEntity_t::UNKNOWN;
}

static void RenderPlayerESP(CCSPlayerController* pPlayerController,
                            const BBox_t& bBox) {
    using namespace esp;

    if (!pPlayerController->m_bPawnIsAlive()) return;

    C_CSPlayerPawn* pPawn = pPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!pPawn) return;

    const bool isLocalPlayer = pPlayerController->m_bIsLocalPlayerController();
    const bool isEnemy =
        pPawn->IsEnemyWithTeam(g_pLocalPlayerController->m_iTeamNum());

    // Filters
    if (bIgnoreTeammates && !isLocalPlayer && !isEnemy)
        return;
    else if (bIgnoreEnemies && isEnemy)
        return;
    else if (bIgnoreSelf && isLocalPlayer)
        return;

    const ImVec2 min = {bBox.x, bBox.y};
    const ImVec2 max = {bBox.w, bBox.h};

    if (bBoxes) {
        g_pBackgroundDrawList->AddRect(min - ImVec2{1.f, 1.f},
                                       max + ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(min + ImVec2{1.f, 1.f},
                                       max - ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(min, max,
                                       isLocalPlayer
                                           ? IM_COL32(52, 131, 235, 255)
                                       : isEnemy ? IM_COL32(255, 0, 0, 255)
                                                 : IM_COL32(0, 255, 0, 255));
    }
    if (bName) {
        const char* szName = pPlayerController->m_sSanitizedPlayerName();
        if (szName && strlen(szName) > 0) {
            const ImVec2 textSize = ImGui::CalcTextSize(szName);
            const ImVec2 textPos = ImFloor(
                {(min.x + max.x - textSize.x) / 2.f, min.y - textSize.y - 2.f});
            g_pBackgroundDrawList->AddText(textPos + ImVec2{1, 1},
                                           IM_COL32(0, 0, 0, 255), szName);
            g_pBackgroundDrawList->AddText(
                textPos, IM_COL32(255, 255, 255, 255), szName);
        }
    }
    if (bHealthbar) {
        const int iClampedHp =
            std::min(pPlayerController->m_iPawnHealth(), 100u);

        const ImVec2 barMin = min - ImVec2{5, 0};
        const ImVec2 barMax = ImVec2{min.x - 2, max.y};

        const float green = iClampedHp * 2.f * 2.55f;
        const float greenClamped = (std::min)(255.f, green);
        const float red = (std::min)(255.f, 510 - green);

        const float height = ((barMax.y - barMin.y) * iClampedHp) / 100.f;

        g_pBackgroundDrawList->AddRectFilled(barMin - ImVec2{0, 1},
                                             barMax + ImVec2{0, 1},
                                             IM_COL32(0, 0, 0, 255));
        if (iClampedHp > 0) {
            g_pBackgroundDrawList->AddRectFilled(
                ImVec2{barMin.x + 1, barMax.y - (std::max)(1.f, height)},
                ImVec2{barMax.x - 1, barMax.y},
                IM_COL32(red, greenClamped, 0, 255));
        }
    }
    if (bActiveWeaponName) {
        CPlayer_WeaponServices* pWeaponServices = pPawn->m_pWeaponServices();
        if (pWeaponServices) {
            C_CSWeaponBase* pActiveWeapon =
                pWeaponServices->m_hActiveWeapon().Get<C_CSWeaponBase>();
            if (pActiveWeapon) {
                RenderWeaponName(pActiveWeapon, bBox);
            }
        }
    }
}

static void RenderWeaponESP(C_CSWeaponBase* pWeapon, const BBox_t& bBox) {
    using namespace esp;

    if (pWeapon->m_hOwnerEntity().IsValid()) return;

    if (fWeaponMaxDistance != 0.f &&
        g_pLocalPlayerPawn->DistanceToSquared(pWeapon) >=
            fWeaponMaxDistance * fWeaponMaxDistance)
        return;

    const ImVec2 min = {bBox.x, bBox.y};
    const ImVec2 max = {bBox.w, bBox.h};

    if (bDroppedWeaponBoxes) {
        g_pBackgroundDrawList->AddRect(min - ImVec2{1.f, 1.f},
                                       max + ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(min + ImVec2{1.f, 1.f},
                                       max - ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(min, max, IM_COL32(255, 255, 255, 255));
    }
    if (bDroppedWeaponName) {
        RenderWeaponName(pWeapon, bBox);
    }
}

static void RenderWeaponName(C_CSWeaponBase* pWeapon, const BBox_t& bBox) {
    // Function to avoid spaghetti code.
    C_AttributeContainer* pAttributeContainer = pWeapon->m_AttributeManager();
    if (!pAttributeContainer) return;

    C_EconItemView* pItemView = pAttributeContainer->m_Item();
    if (!pItemView) return;

    CEconItemDefinition* pItemStaticData = pItemView->GetStaticData();
    if (!pItemStaticData) return;

    const char* szWeaponName =
        interfaces::pLocalize->FindSafe(pItemStaticData->m_pszItemBaseName);
    if (!szWeaponName || strlen(szWeaponName) < 1) return;

    const ImVec2 min = {bBox.x, bBox.y};
    const ImVec2 max = {bBox.w, bBox.h};

    const ImVec2 textSize = ImGui::CalcTextSize(szWeaponName);
    const ImVec2 textPos = ImFloor(
        {(min.x + max.x - textSize.x) / 2.f, max.y + textSize.y - 12.f});
    g_pBackgroundDrawList->AddText(textPos + ImVec2{1, 1},
                                   IM_COL32(0, 0, 0, 255), szWeaponName);
    g_pBackgroundDrawList->AddText(textPos, IM_COL32(255, 255, 255, 255),
                                   szWeaponName);
}

static void RenderChickenESP(C_Chicken* pChicken, const BBox_t& bBox) {
    using namespace esp;

    CHandle hLeader = pChicken->m_leader();

    const ImVec2 min = {bBox.x, bBox.y};
    const ImVec2 max = {bBox.w, bBox.h};

    if (bChickenBox) {
        g_pBackgroundDrawList->AddRect(min - ImVec2{1.f, 1.f},
                                       max + ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(min + ImVec2{1.f, 1.f},
                                       max - ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(
            min, max,
            IM_COL32(hLeader == g_pLocalPlayerPawn->GetRefEHandle() ? 160 : 255,
                     0, 255, 255));
    }
}
