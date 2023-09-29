#pragma once

#include "c_baseplayerweapon.hpp"

class C_CSWeaponBase : public C_BasePlayerWeapon {
   public:
    void AddStattrakEntity();
    void AddNametagEntity();

    SCHEMA_FIELD(m_bUIWeapon, "C_CSWeaponBase", "m_bUIWeapon", bool);
    SCHEMA_FIELD(m_iOriginalTeamNumber, "C_CSWeaponBase",
                 "m_iOriginalTeamNumber", int);
    PSCHEMA_FIELD_OFFSET(m_hStattrakAttachment, "C_CSWeaponBase",
                         "m_iNumEmptyAttacks", 4, void);
    PSCHEMA_FIELD_OFFSET(m_hNametagAttachment, "C_CSWeaponBase",
                         "m_iNumEmptyAttacks", 20, void);
};
