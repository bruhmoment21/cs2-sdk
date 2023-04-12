#pragma once

#include "../../schema/schema.hpp"

#include "../types/chandle.hpp"

#include "c_baseentity.hpp"

class C_Chicken : public C_BaseEntity {
   public:
    SCHEMA_FIELD(m_leader, "C_Chicken", "m_leader", CHandle);
};
