#pragma once

#include "../../schema/schema.hpp"

#include "../econ/ceconitemdefinition.hpp"

class C_EconItemView {
   public:
    auto GetStaticData() {
        return CALL_VIRTUAL(CEconItemDefinition*, 13, this);
    }

    SCHEMA_FIELD(m_iItemDefinitionIndex, "C_EconItemView",
                 "m_iItemDefinitionIndex", uint16_t);
    SCHEMA_FIELD(m_iItemID, "C_EconItemView", "m_iItemID", uint64_t);
    SCHEMA_FIELD(m_iItemIDLow, "C_EconItemView", "m_iItemIDLow", uint32_t);
    SCHEMA_FIELD(m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh", uint32_t);
    SCHEMA_FIELD(m_iAccountID, "C_EconItemView", "m_iAccountID", uint32_t);
};
