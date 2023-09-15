#pragma once

#include "../interfaces/cschemasystem.hpp"

#include "centityidentity.hpp"

class CEntityInstance {
   public:
    auto Schema_DynamicBinding() {
        SchemaClassInfoData_t* rv = nullptr;
        CALL_VIRTUAL(void, 0, this, &rv);
        return rv;
    }

    auto GetRefEHandle() {
        CHandle handle;
        CALL_VIRTUAL(void*, 2, this, &handle);
        return handle;
    }

    SCHEMA_FIELD(m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*);
};
