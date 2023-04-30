#include "cgameentitysystem.hpp"

#include "../../interfaces/interfaces.hpp"

CGameEntitySystem* CGameEntitySystem::GetInstance() {
    if (!interfaces::pGameResourceService) return nullptr;
    return interfaces::pGameResourceService->GetGameEntitySystem();
}

CCSPlayerController* CGameEntitySystem::GetLocalPlayerController() {
    if (!memory::fnGetLocalPlayerController) return nullptr;
    return memory::fnGetLocalPlayerController(-1);
}
