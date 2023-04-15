#pragma once

#include "../../schema/schema.hpp"

#include "../types/chandle.hpp"

#include "c_basemodelentity.hpp"

class C_Chicken : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_leader, "C_Chicken", "m_leader", CHandle);
};
