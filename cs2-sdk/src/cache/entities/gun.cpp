#include "pch.hpp"

#include <cache/entities/gun.hpp>

#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

bool CCachedGun::CanDoESP() {
    C_BaseEntity* weapon = Get();
    if (!weapon || weapon->m_hOwnerEntity().IsValid()) {
        return false;
    }

    return true;
}

void CCachedGun::DrawESP() {
    if (g_Vars.m_WeaponBoxes) {
        DrawBoundingBox(IM_COL32(255, 255, 0, 255));
    }
}
