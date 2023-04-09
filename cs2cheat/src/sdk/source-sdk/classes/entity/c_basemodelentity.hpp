#pragma once

#include "c_baseentity.hpp"

class C_BaseModelEntity : public C_BaseEntity {
   public:
    Vector GetEyePosition();
    SCHEMA_FIELD(m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset",
                 Vector);
};
