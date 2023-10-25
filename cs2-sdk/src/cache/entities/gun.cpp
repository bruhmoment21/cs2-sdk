#include "pch.hpp"

#include <cache/entities/gun.hpp>

#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

bool CCachedGun::CanDrawESP() {
    if (!g_Vars.m_WeaponESP) {
        return false;
    }

    C_BaseEntity* weapon = Get<C_BaseEntity>();
    if (!weapon || weapon->m_hOwnerEntity().IsValid()) {
        return false;
    }

    return true;
}
