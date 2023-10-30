#pragma once

#include <bindings/baseentity.hpp>

class C_Hostage : public C_BaseEntity {
   public:
    SCHEMA(CHandle<C_BaseEntity>, m_leader, "C_Hostage", "m_leader");
};
