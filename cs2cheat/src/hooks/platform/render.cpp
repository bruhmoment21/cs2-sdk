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

        ImGui::Text("%d. %s; hp: %u; steamid: %llu\n", i,
                    pPlayer->m_sSanitizedPlayerName(), pPlayer->m_iPawnHealth(),
                    pPlayer->m_steamID());
    }

    ImGui::End();
}
