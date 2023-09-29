#pragma once

#include "../../schema/schema.hpp"
#include "../../math/math.hpp"

#include "../types/chandle.hpp"

class CEntityIdentity {
   public:
    SCHEMA_FIELD(m_designerName, "CEntityIdentity", "m_designerName",
                 const char*);
    SCHEMA_FIELD(m_flags, "CEntityIdentity", "m_flags", uint32_t);
};
