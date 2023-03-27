#include <imgui/imgui.h>

#include "../../sdk/memory/memory.hpp"
#include "../../sdk/interfaces/interfaces.hpp"

void CS2_RenderUI() {
    static bool bShow = true;
    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) bShow = !bShow;
    if (!bShow) return;

    ImGui::Begin("menu");
    ImGui::TextUnformatted("Players:");

    for (int i = 1; i <= 64; ++i) {
        CCSPlayerController* pPlayer =
            interfaces::pEntitySystem->GetBaseEntity<CCSPlayerController>(i);
        if (!pPlayer || !pPlayer->IsPlayerController()) continue;

        C_CSPlayerPawn* pPawn =
            pPlayer->m_hPlayerPawn().GetAs<C_CSPlayerPawn>();
        if (!pPawn) continue;
        Vector absOrigin = pPawn->m_pGameSceneNode()->m_vecAbsOrigin();
        ImGui::Text("%d. [%f, %f, %f] %s; hp: %u; steamid: %llu\n", i,
                    absOrigin.x, absOrigin.y, absOrigin.z,
                    pPlayer->m_sSanitizedPlayerName(), pPlayer->m_iPawnHealth(),
                    pPlayer->m_steamID());
    }

    ImGui::End();
}
