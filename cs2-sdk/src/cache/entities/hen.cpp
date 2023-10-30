#include "pch.hpp"

#include <cache/entities/hen.hpp>

#include <vars/vars.hpp>

void CCachedHen::DrawESP() {
    if (g_Vars.m_ChickenBoxes) {
        DrawBoundingBox(IM_COL32(206, 66, 245, 255));
    }
}
