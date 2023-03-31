#pragma once

#include "../../../math/math.hpp"

#include "../../schema/schema.hpp"

class CCollisionProperty {
   public:
    SCHEMA_FIELD(m_vecMins, "CCollisionProperty", "m_vecMins", Vector);
    SCHEMA_FIELD(m_vecMaxs, "CCollisionProperty", "m_vecMaxs", Vector);
};
