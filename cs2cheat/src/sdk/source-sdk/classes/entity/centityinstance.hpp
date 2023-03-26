#pragma once

#include "centityidentity.hpp"

class CEntityInstance {
   public:
    NETVAR(m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*);
};
