#include "pch.hpp"

#include <cache/entities/player.hpp>
#include <cache/cache.hpp>

#include <renderer/renderer.hpp>
#include <vars/vars.hpp>

#include <bindings/playercontroller.hpp>
#include <bindings/playerpawn.hpp>

#include <interfaces/engineclient.hpp>
#include <interfaces/cvar.hpp>

#include <imgui/imgui_internal.h>

bool CCachedPlayer::CanDoESP() {
    CCSPlayerController* controller = Get();
    if (!controller || !controller->m_bPawnIsAlive()) {
        return false;
    }

    C_CSPlayerPawnBase* pawn = controller->m_hPawn().Get();
    if (!pawn || pawn->IsObserverPawn()) {
        return false;
    }

    return true;
}

void CCachedPlayer::DrawESP() {
    CCachedPlayer* cachedLocalPlayer = CMatchCache::Get().GetLocalPlayer();
    if (!cachedLocalPlayer) {
        return InvalidateDrawInfo();
    }

    auto drawList = CRenderer::GetBackgroundDrawList();

    const ImVec2& min = m_BBox.m_Mins;
    const ImVec2& max = m_BBox.m_Maxs;

    CCSPlayerController* controller = Get();
    C_CSPlayerPawnBase* pawn = controller->m_hPawn().Get();

    if (g_Vars.m_PlayerBoxes) {
        DrawBoundingBox([this, cachedLocalPlayer]() {
            if (IsLocalPlayer()) {
                return IM_COL32(52, 131, 235, 255);
            } else if (IsEnemyWithTeam(cachedLocalPlayer->GetTeam())) {
                return IM_COL32(255, 0, 0, 255);
            }

            return IM_COL32(0, 255, 0, 255);
        }());
    }

    if (g_Vars.m_PlayerNames) {
        const char* playerName = controller->m_sSanitizedPlayerName();
        if (playerName) {
            const ImVec2 textSize = ImGui::CalcTextSize(playerName);
            const ImVec2 textPos = ImTrunc({(min.x + max.x - textSize.x) * 0.5f, min.y - textSize.y - 2.f});

            drawList->AddText(textPos + ImVec2{1, 1}, IM_COL32(0, 0, 0, 255), playerName);
            drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), playerName);
        }
    }

    if (g_Vars.m_PlayerHealthBar) {
        const int clampedHp = std::min(pawn->m_iHealth(), 100);

        const ImVec2 barMin = min - ImVec2{5, 0};
        const ImVec2 barMax{min.x - 2, max.y};

        const float green = clampedHp * 2.f * 2.55f;
        const float greenClamped = (std::min)(255.f, green);
        const float red = (std::min)(255.f, 510 - green);

        const float height = ((barMax.y - barMin.y) * clampedHp) / 100.f;

        drawList->AddRectFilled(barMin - ImVec2{0, 1}, barMax + ImVec2{0, 1}, IM_COL32(0, 0, 0, 255));
        if (clampedHp > 0) {
            drawList->AddRectFilled({barMin.x + 1, barMax.y - (std::max)(1.f, height)}, {barMax.x - 1, barMax.y},
                                    IM_COL32(red, greenClamped, 0, 255));
        }
    }
}

void CCachedPlayer::CalculateDrawInfo() {
    CCSPlayerController* controller = Get();
    C_BaseEntity* pawn = controller->m_hPawn().Get();
    if (!pawn->CalculateBBoxByCollision(m_BBox)) {
        return InvalidateDrawInfo();
    }
}

CCachedPlayer::Team CCachedPlayer::GetTeam() {
    CCSPlayerController* controller = Get();
    if (!controller) return Team::UNKNOWN;

    C_CSPlayerPawnBase* pawn = controller->m_hPawn().Get();
    if (!pawn) return Team::UNKNOWN;

    return static_cast<Team>(pawn->m_iTeamNum());
}

bool CCachedPlayer::IsEnemyWithTeam(Team team) {
    static ConVar* mp_teammates_are_enemies = CCVar::Get()->GetCvarByName("mp_teammates_are_enemies");
    return mp_teammates_are_enemies->GetValue<bool>() ? true : GetTeam() != team;
}

bool CCachedPlayer::IsLocalPlayer() { return GetIndex() == CEngineClient::Get()->GetLocalPlayer(); }
