#pragma once

#include "cplayer_weaponservices.hpp"
#include "c_basemodelentity.hpp"

class C_BasePlayerPawn : public C_BaseModelEntity {
   public:
    SCHEMA_FIELD(m_hController, "C_BasePlayerPawn", "m_hController", CHandle);
    SCHEMA_FIELD(m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices",
                 CPlayer_WeaponServices*);
};
