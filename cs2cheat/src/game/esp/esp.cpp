#include "esp.hpp"

#include "../../sdk/interfaces/interfaces.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

static ImDrawList* g_pBackgroundDrawList = nullptr;
static CCSPlayerController* g_pLocalPlayerController = nullptr;
static C_CSPlayerPawn* g_pLocalPlayerPawn = nullptr;

static void RenderPlayerESP(CCSPlayerController* pPlayerController);
static void RenderWeaponESP(C_WeaponCSBase* pWeapon);
static void RenderWeaponName(C_WeaponCSBase* pWeapon, const BBox_t& bBox);
static void RenderChickenESP(C_Chicken* pChicken);

void esp::Render() {
    if (!interfaces::pEngine->IsInGame()) return;

    CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance();
    if (!pEntitySystem) return;

    g_pBackgroundDrawList = ImGui::GetBackgroundDrawList();

    g_pLocalPlayerController = pEntitySystem->GetLocalPlayerController();
    if (!g_pLocalPlayerController) return;

    g_pLocalPlayerPawn =
        g_pLocalPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!g_pLocalPlayerPawn) return;

    // Expand ESP as needed.
    int highestIndex = pEntitySystem->GetHighestEntityIndex();
    for (int i = 1; i <= highestIndex; ++i) {
        C_BaseEntity* pEntity = pEntitySystem->GetBaseEntity(i);
        if (!pEntity) continue;

        if (pEntity->IsBasePlayerController())
            RenderPlayerESP((CCSPlayerController*)pEntity);
        else if (pEntity->IsBasePlayerWeapon())
            RenderWeaponESP((C_WeaponCSBase*)pEntity);
        else if (pEntity->IsChicken())
            RenderChickenESP((C_Chicken*)pEntity);
    }
}

static void RenderPlayerESP(CCSPlayerController* pPlayerController) {
    using namespace esp;

    if (!pPlayerController->m_bPawnIsAlive()) return;

    C_CSPlayerPawn* pPawn = pPlayerController->m_hPawn().Get<C_CSPlayerPawn>();
    if (!pPawn) return;

    const bool isEnemy =
        pPawn->IsEnemyWithTeam(g_pLocalPlayerController->m_iTeamNum());
    if (bIgnoreTeammates && !isEnemy) return;

    BBox_t bBox;
    if (!pPawn->GetBoundingBox(bBox)) return;

    const ImVec2 min = {bBox.x, bBox.y};
    const ImVec2 max = {bBox.w, bBox.h};

    if (bBoxes) {
        g_pBackgroundDrawList->AddRect(min - ImVec2{1.f, 1.f},
                                       max + ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(min + ImVec2{1.f, 1.f},
                                       max - ImVec2{1.f, 1.f},
                                       IM_COL32(0, 0, 0, 255));
        g_pBackgroundDrawList->AddRect(
            min, max,
            isEnemy ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 255, 0, 255));
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
            C_WeaponCSBase* pActiveWeapon =
                pWeaponServices->m_hActiveWeapon().Get<C_WeaponCSBase>();
            if (pActiveWeapon) {
                RenderWeaponName(pActiveWeapon, bBox);
            }
        }
    }
}

static void RenderWeaponESP(C_WeaponCSBase* pWeapon) {
    using namespace esp;

    if (pWeapon->m_hOwnerEntity().IsValid()) return;

    if (fWeaponMaxDistance != 0.f &&
        g_pLocalPlayerPawn->DistanceToSquared(pWeapon) >=
            fWeaponMaxDistance * fWeaponMaxDistance)
        return;

    BBox_t bBox;
    if (!pWeapon->GetBoundingBox(bBox, true)) return;

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

static void RenderWeaponName(C_WeaponCSBase* pWeapon, const BBox_t& bBox) {
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

static void RenderChickenESP(C_Chicken* pChicken) {
    using namespace esp;

    BBox_t bBox;
    if (!pChicken->GetBoundingBox(bBox)) return;

    C_CSPlayerPawnBase* pLeaderPawn =
        pChicken->m_leader().Get<C_CSPlayerPawnBase>();

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
            IM_COL32(pLeaderPawn == g_pLocalPlayerPawn ? 160 : 255, 0, 255,
                     255));
    }
}
