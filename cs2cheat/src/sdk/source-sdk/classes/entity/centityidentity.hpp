#pragma once

#include "../../../math/math.hpp"

#include "../../schema/schema.hpp"

#include "chandle.hpp"

class CEntityIdentity {
   public:
    SCHEMA_FIELD(m_designerName, "CEntityIdentity", "m_designerName",
                 const char*);
};
