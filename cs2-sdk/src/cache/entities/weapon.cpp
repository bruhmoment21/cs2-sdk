#include "pch.hpp"

#include <cache/entities/weapon.hpp>

#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

#include <imgui/imgui.h>

CCachedWeapon::CCachedWeapon(CBaseHandle handle) : CCachedBaseEntity(handle) { m_BoxColor = IM_COL32(255, 255, 0, 255); }

bool CCachedWeapon::CanDrawESP() {
    if (!g_Vars.m_WeaponESP) {
        return false;
    }

    C_BaseEntity* weapon = Get<C_BaseEntity>();
    if (!weapon || weapon->m_hOwnerEntity().IsValid()) {
        return false;
    }

    return true;
}
