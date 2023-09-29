#pragma once

#include "../interfaces/cschemasystem.hpp"

#include "centityidentity.hpp"

class CEntityInstance {
   public:
    auto Schema_DynamicBinding() {
        SchemaClassInfoData_t* rv = nullptr;
        CALL_VIRTUAL(void, 34, this, &rv);
        return rv;
    }

    auto GetRefEHandle() {
        CEntityIdentity* pIdentity = m_pEntity();
        auto v3 = *(uint32_t*)(pIdentity + 16);
        auto v4 = ENT_ENTRY_MASK;
        auto v5 = ((v3 >> 15) - (*(uint32_t*)(pIdentity + 48) & 1)) << 15;
        if (v3 != -1) {
            v4 = *(uint32_t*)(pIdentity + 16) & ENT_ENTRY_MASK;
        }

        return CHandle(v4 | v5);
    }

    SCHEMA_FIELD(m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*);
};
