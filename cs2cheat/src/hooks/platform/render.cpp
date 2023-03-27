#include "../../sdk/interfaces/interfaces.hpp"
#include "../../sdk/memory/memory.hpp"
#include "../../utils/utils.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

void CS2_RenderUI() {
    static bool bBoxEsp, bNameEsp, bHealthbar;
    ImDrawList* pBackgroundDrawList = ImGui::GetBackgroundDrawList();
    if (ImGui::IsKeyPressed(ImGuiKey_End, false)) utils::UnloadLibrary();

    for (int i = 1; i <= MAX_PLAYERS; ++i) {
        CCSPlayerController* pPlayer =
            interfaces::pEntitySystem->GetBaseEntity<CCSPlayerController>(i);
        if (!pPlayer || !pPlayer->IsPlayerController() ||
            !pPlayer->m_bPawnIsAlive())
            continue;

        C_CSPlayerPawn* pPawn = pPlayer->m_hPawn().GetAs<C_CSPlayerPawn>();
        if (!pPawn) continue;

        BBox_t bBox;
        if (!pPawn->GetBoundingBox(bBox)) continue;

        const ImVec2 min = {bBox.x, bBox.y};
        const ImVec2 max = {bBox.w, bBox.h};

        if (bBoxEsp) {
            pBackgroundDrawList->AddRect(min, max,
                                         IM_COL32(255, 255, 255, 255));
            pBackgroundDrawList->AddRect(min - ImVec2{1.f, 1.f},
                                         max + ImVec2{1.f, 1.f},
                                         IM_COL32(0, 0, 0, 255));
            pBackgroundDrawList->AddRect(min + ImVec2{1.f, 1.f},
                                         max - ImVec2{1.f, 1.f},
                                         IM_COL32(0, 0, 0, 255));
        }
        if (bNameEsp) {
            const char* szName = pPlayer->m_sSanitizedPlayerName();
            if (szName && strlen(szName) > 0) {
                const ImVec2 textSize = ImGui::CalcTextSize(szName);
                const ImVec2 textPos =
                    ImFloor({(min.x + max.x - textSize.x) / 2.f,
                             min.y - textSize.y - 2.f});
                pBackgroundDrawList->AddText(textPos + ImVec2{1, 1},
                                             IM_COL32(0, 0, 0, 255), szName);
                pBackgroundDrawList->AddText(
                    textPos, IM_COL32(255, 255, 255, 255), szName);
            }
        }
        if (bHealthbar) {
            const int iClampedHp = std::min(pPlayer->m_iPawnHealth(), 100u);

            const ImVec2 barMin = min - ImVec2{5, 0};
            const ImVec2 barMax = ImVec2{min.x - 2, max.y};

            const float green = iClampedHp * 2.f * 2.55f;
            const float greenClamped = (std::min)(255.f, green);
            const float red = (std::min)(255.f, 510 - green);

            const float height = ((barMax.y - barMin.y) * iClampedHp) / 100.f;

            pBackgroundDrawList->AddRectFilled(barMin - ImVec2{0, 1},
                                               barMax + ImVec2{0, 1},
                                               IM_COL32(0, 0, 0, 255));
            if (iClampedHp > 0) {
                pBackgroundDrawList->AddRectFilled(
                    ImVec2{barMin.x + 1, barMax.y - (std::max)(1.f, height)},
                    ImVec2{barMax.x - 1, barMax.y},
                    IM_COL32(red, greenClamped, 0, 255));
            }
        }
    }

    static bool bShow = false;
    if (ImGui::IsKeyPressed(ImGuiKey_Insert, false)) bShow = !bShow;
    if (!bShow) return;

    ImGuiIO& IO = ImGui::GetIO();
    ImGui::SetNextWindowPos(IO.DisplaySize / 2.f, ImGuiCond_Once, {0.5f, 0.5f});
    ImGui::Begin("cs2sdk menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("Box", &bBoxEsp);
    ImGui::Checkbox("Name", &bNameEsp);
    ImGui::Checkbox("Healthbar", &bHealthbar);
    if (ImGui::Button("Unload", {128, 0})) utils::UnloadLibrary();
    ImGui::End();
}
