#pragma once

#include "c_baseplayerweapon.hpp"

class C_WeaponCSBase : public C_BasePlayerWeapon {
   public:
    SCHEMA_FIELD(m_iOriginalTeamNumber, "C_WeaponCSBase",
                 "m_iOriginalTeamNumber", int);
};
