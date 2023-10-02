#include "pch.hpp"

#include <cache/entities/base.hpp>

#include <renderer/renderer.hpp>
#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

#include <imgui/imgui.h>

CCachedBaseEntity::CCachedBaseEntity(CBaseHandle handle) {
    m_Handle = handle;

    m_BoxColor = IM_COL32(255, 255, 255, 255);
    m_BoxOutlineColor = IM_COL32(0, 0, 0, 255);
}

bool CCachedBaseEntity::CanDrawESP() { return g_Vars.m_OtherESP && Get() != nullptr; }

void CCachedBaseEntity::ResetESP() { m_BBox.Invalidate(); }

void CCachedBaseEntity::RenderESP() {
    auto drawList = CRenderer::GetBackgroundDrawList();

    if (g_Vars.m_Use3DBoxes) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 1; j <= 4; j <<= 1) {
                if (!(i & j)) drawList->AddLine(m_BBox.m_Vertices[i], m_BBox.m_Vertices[i + j], m_BoxOutlineColor, 2.5f);
            }
        }

        for (int i = 0; i < 8; ++i) {
            for (int j = 1; j <= 4; j <<= 1) {
                if (!(i & j)) drawList->AddLine(m_BBox.m_Vertices[i], m_BBox.m_Vertices[i + j], m_BoxColor);
            }
        }
    } else {
        const ImVec2& min = m_BBox.m_Mins;
        const ImVec2& max = m_BBox.m_Maxs;

        drawList->AddRect(min - ImVec2{1.f, 1.f}, max + ImVec2{1.f, 1.f}, m_BoxOutlineColor);
        drawList->AddRect(min + ImVec2{1.f, 1.f}, max - ImVec2{1.f, 1.f}, m_BoxOutlineColor);
        drawList->AddRect(min, max, m_BoxColor);
    }
}

void CCachedBaseEntity::UpdateESP() {
    C_BaseEntity* ent = Get();

    if (!ent->CalculateBBoxByHitbox(m_BBox)) {
        return ResetESP();
    }
}
