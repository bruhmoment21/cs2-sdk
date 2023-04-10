#pragma once

#include "../../schema/schema.hpp"
#include "../../math/math.hpp"

class CCollisionProperty {
   public:
    SCHEMA_FIELD(m_vecMins, "CCollisionProperty", "m_vecMins", Vector);
    SCHEMA_FIELD(m_vecMaxs, "CCollisionProperty", "m_vecMaxs", Vector);
};
