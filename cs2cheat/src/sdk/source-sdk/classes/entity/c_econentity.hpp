#pragma once

#include "c_attributecontainer.hpp"
#include "c_baseentity.hpp"

class C_EconEntity : public C_BaseEntity {
   public:
    PSCHEMA_FIELD(m_AttributeManager, "C_EconEntity", "m_AttributeManager",
                  C_AttributeContainer);
    SCHEMA_FIELD(m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit",
                 int32_t);
    SCHEMA_FIELD(m_nFallbackSeed, "C_EconEntity", "m_nFallbackSeed", int32_t);
    SCHEMA_FIELD(m_flFallbackWear, "C_EconEntity", "m_flFallbackWear", float);
};
