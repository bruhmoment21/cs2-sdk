#include "pch.hpp"

#include <cache/entities/captive.hpp>

#include <vars/vars.hpp>

#include <bindings/hostage.hpp>

bool CCachedCaptive::CanDoESP() {
    C_Hostage* hostage = Get();
    if (!hostage || hostage->m_leader().IsValid()) {
        return false;
    }

    return true;
}

void CCachedCaptive::DrawESP() {
    if (g_Vars.m_HostageBoxes) {
        DrawBoundingBox(IM_COL32(255, 119, 0, 255));
    }
}
