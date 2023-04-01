#pragma once

#include "../../schema/schema.hpp"

class C_EconItemView {
   public:
    SCHEMA_FIELD(m_iItemDefinitionIndex, "C_EconItemView",
                 "m_iItemDefinitionIndex", uint16_t);
    SCHEMA_FIELD(m_iItemIDLow, "C_EconItemView", "m_iItemIDLow", uint32_t);
};
