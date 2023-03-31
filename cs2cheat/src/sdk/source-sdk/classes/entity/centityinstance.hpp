#pragma once

#include "centityidentity.hpp"

class CEntityInstance {
   public:
    SCHEMA_FIELD(m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*);
};
