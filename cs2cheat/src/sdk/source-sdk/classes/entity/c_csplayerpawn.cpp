#include "c_csplayerpawn.hpp"

#include "../../../interfaces/interfaces.hpp"
#include "../../../memory/memory.hpp"

bool C_CSPlayerPawn::IsEnemyToLocalPlayer(int lpTeam) {
    static ConVar* mp_teammates_are_enemies =
        interfaces::pCvar->FindVarByName("mp_teammates_are_enemies");

    CCSPlayerController* pPlayerController =
        m_hController().GetAs<CCSPlayerController>();
    if (!pPlayerController) return true;

    if (pPlayerController->m_bIsLocalPlayerController()) return false;
    if (mp_teammates_are_enemies->GetValue<bool>()) return true;

    return m_iTeamNum() != lpTeam;
}
