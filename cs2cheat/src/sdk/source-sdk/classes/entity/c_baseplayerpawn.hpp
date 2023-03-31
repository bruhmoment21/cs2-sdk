#pragma once

#include "c_baseentity.hpp"

class C_BasePlayerPawn : public C_BaseEntity {
   public:
    SCHEMA_FIELD(m_hController, "C_BasePlayerPawn", "m_hController", CHandle);
};
