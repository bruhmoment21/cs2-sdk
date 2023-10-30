#include "pch.hpp"

#include <cache/entities/base.hpp>

#include <renderer/renderer.hpp>
#include <vars/vars.hpp>

#include <bindings/baseentity.hpp>

#include <imgui/imgui.h>

bool CCachedBaseEntity::CanDoESP() { return Get() != nullptr; }

void CCachedBaseEntity::DrawESP() {
    if (g_Vars.m_OtherBoxes) {
        DrawBoundingBox(IM_COL32(255, 255, 255, 255));
    }
}

void CCachedBaseEntity::InvalidateDrawInfo() { m_BBox.Invalidate(); }

void CCachedBaseEntity::CalculateDrawInfo() {
    C_BaseEntity* ent = Get();

    if (!ent->CalculateBBoxByHitbox(m_BBox)) {
        return InvalidateDrawInfo();
    }
}

void CCachedBaseEntity::DrawBoundingBox(ImU32 color) {
    static constexpr auto outlineColor = IM_COL32(0, 0, 0, 255);

    auto drawList = CRenderer::GetBackgroundDrawList();

    if (g_Vars.m_Use3DBoxes) {
        for (int i = 0; i < 8; ++i) {
            for (int j = 1; j <= 4; j <<= 1) {
                if (!(i & j)) drawList->AddLine(m_BBox.m_Vertices[i], m_BBox.m_Vertices[i + j], outlineColor, 2.5f);
            }
        }

        for (int i = 0; i < 8; ++i) {
            for (int j = 1; j <= 4; j <<= 1) {
                if (!(i & j)) drawList->AddLine(m_BBox.m_Vertices[i], m_BBox.m_Vertices[i + j], color);
            }
        }
    } else {
        const ImVec2& min = m_BBox.m_Mins;
        const ImVec2& max = m_BBox.m_Maxs;

        drawList->AddRect(min - ImVec2{1.f, 1.f}, max + ImVec2{1.f, 1.f}, outlineColor);
        drawList->AddRect(min + ImVec2{1.f, 1.f}, max - ImVec2{1.f, 1.f}, outlineColor);
        drawList->AddRect(min, max, color);
    }
}
