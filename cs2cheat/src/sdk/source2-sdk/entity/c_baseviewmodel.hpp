#pragma once

#include "c_basemodelentity.hpp"

class C_BaseViewModel : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_hWeapon, "C_BaseViewModel", "m_hWeapon", CHandle);
};
