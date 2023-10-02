#pragma once

#include <schemamgr/schema_manager.hpp>

#include <bindings/entityidentity.hpp>

#include <types/handle.hpp>

class CSchemaClassInfo;

class CEntityInstance {
   public:
    CSchemaClassInfo* Schema_DynamicBinding();
    CBaseHandle GetRefEHandle();

    SCHEMA(CEntityIdentity*, m_pEntity, "CEntityInstance", "m_pEntity");
};
