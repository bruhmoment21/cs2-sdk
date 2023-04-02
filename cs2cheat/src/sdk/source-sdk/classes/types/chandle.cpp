#include "chandle.hpp"

#include "../../../interfaces/interfaces.hpp"

C_BaseEntity* CHandle::GetBaseEntity() const {
    return interfaces::pEntitySystem->GetBaseEntity(GetEntryIndex());
}
