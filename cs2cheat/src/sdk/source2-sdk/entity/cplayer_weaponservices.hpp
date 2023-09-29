#pragma once

#include "../../schema/schema.hpp"

#include "../types/cnetworkutlvectorbase.hpp"
#include "../types/chandle.hpp"

#include "c_csweaponbase.hpp"

class CPlayer_WeaponServices {
   public:
    SCHEMA_FIELD(m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon",
                 CHandle);
};
