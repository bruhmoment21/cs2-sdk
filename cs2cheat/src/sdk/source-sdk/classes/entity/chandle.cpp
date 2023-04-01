#include "chandle.hpp"

#include "../../../interfaces/interfaces.hpp"

C_BaseEntity* CHandle::Get() const {
    return interfaces::pEntitySystem->GetBaseEntityFromHandle(*this);
}
