#pragma once

#include "../../schema/schema.hpp"

#include "../types/cnetworkutlvectorbase.hpp"

#include "c_baseplayerweapon.hpp"
#include "chandle.hpp"

class CPlayer_WeaponServices {
   public:
    SCHEMA_FIELD(m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon",
                 CHandle);
    PSCHEMA_FIELD(m_hMyWeapons, "CPlayer_WeaponServices", "m_hMyWeapons",
                  CNetworkUtlVectorBase<CHandle>);
};
