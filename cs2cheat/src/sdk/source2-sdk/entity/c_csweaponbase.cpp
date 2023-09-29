#include "c_csweaponbase.hpp"

#include "../../memory/memory.hpp"

void C_CSWeaponBase::AddStattrakEntity() {
    if (!memory::fnAddStattrakEntity) return;
    return memory::fnAddStattrakEntity(m_hStattrakAttachment());
}

void C_CSWeaponBase::AddNametagEntity() {
    if (!memory::fnAddNametagEntity) return;
    return memory::fnAddNametagEntity(m_hNametagAttachment());
}
