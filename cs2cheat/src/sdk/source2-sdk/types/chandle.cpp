#include "chandle.hpp"

#include "../../interfaces/interfaces.hpp"

C_BaseEntity* CHandle::GetBaseEntity() const {
    CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance();
    if (!pEntitySystem) return nullptr;

    return pEntitySystem->GetBaseEntity(GetEntryIndex());
}
