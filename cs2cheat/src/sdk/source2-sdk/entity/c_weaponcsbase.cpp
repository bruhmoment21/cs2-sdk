#include "c_weaponcsbase.hpp"

#include "../../memory/memory.hpp"

void C_WeaponCSBase::AddStattrakEntity() {
    if (!memory::fnAddStattrakEntity) return;
    return memory::fnAddStattrakEntity(m_hStattrakAttachment());
}

void C_WeaponCSBase::AddNametagEntity() {
    if (!memory::fnAddNametagEntity) return;
    return memory::fnAddNametagEntity(m_hNametagAttachment());
}
