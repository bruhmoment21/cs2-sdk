#include "c_csplayerpawn.hpp"

#include "../../interfaces/interfaces.hpp"
#include "../../memory/memory.hpp"

bool C_CSPlayerPawn::IsEnemyWithTeam(int team) {
    static ConVar* mp_teammates_are_enemies =
        interfaces::pCvar->FindVarByName("mp_teammates_are_enemies");

    CCSPlayerController* pPlayerController =
        m_hController().Get<CCSPlayerController>();

    if (pPlayerController && pPlayerController->m_bIsLocalPlayerController())
        return false;

    return mp_teammates_are_enemies->GetValue<bool>() ? true
                                                      : m_iTeamNum() != team;
}
