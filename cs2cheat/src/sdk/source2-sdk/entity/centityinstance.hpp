#pragma once

#include "../interfaces/cschemasystem.hpp"

#include "centityidentity.hpp"

class CEntityInstance {
   public:
    auto Schema_DynamicBinding() {
        return CALL_VIRTUAL(SchemaClassInfoData_t*, 0, this);
    }

    auto GetRefEHandle() {
        CHandle handle;
        CALL_VIRTUAL(void*, 2, this, &handle);
        return handle;
    }

    SCHEMA_FIELD(m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*);
};
