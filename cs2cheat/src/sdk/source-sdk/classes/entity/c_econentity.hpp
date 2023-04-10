#pragma once

#include "c_attributecontainer.hpp"
#include "c_basemodelentity.hpp"

class C_EconEntity : public C_BaseModelEntity {
   public:
    PSCHEMA_FIELD(m_AttributeManager, "C_EconEntity", "m_AttributeManager",
                  C_AttributeContainer);
    SCHEMA_FIELD(m_OriginalOwnerXuidLow, "C_EconEntity",
                 "m_OriginalOwnerXuidLow", uint32_t);
    SCHEMA_FIELD(m_OriginalOwnerXuidHigh, "C_EconEntity",
                 "m_OriginalOwnerXuidHigh", uint32_t);

    uint64_t GetOriginalOwnerXuid() {
        return ((uint64_t)(m_OriginalOwnerXuidHigh()) << 32) |
               m_OriginalOwnerXuidLow();
    }
};
