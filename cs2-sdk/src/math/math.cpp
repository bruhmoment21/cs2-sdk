#include "pch.hpp"

#include <math/math.hpp>

#include <imgui/imgui_internal.h>

void CMath::UpdateViewMatrix(VMatrix* viewMatrix) {
    if (!viewMatrix) return;
    m_ViewMatrix = *viewMatrix;
}

bool CMath::WorldToScreen(const Vector& in, ImVec2& out) {
    if (!ImGui::GetCurrentContext()) return false;

    const float z = m_ViewMatrix[3][0] * in.x + m_ViewMatrix[3][1] * in.y + m_ViewMatrix[3][2] * in.z + m_ViewMatrix[3][3];
    if (z < 0.001f) return false;

    out = ImGui::GetIO().DisplaySize * 0.5f;
    out.x *= 1.0f + (m_ViewMatrix[0][0] * in.x + m_ViewMatrix[0][1] * in.y + m_ViewMatrix[0][2] * in.z + m_ViewMatrix[0][3]) / z;
    out.y *= 1.0f - (m_ViewMatrix[1][0] * in.x + m_ViewMatrix[1][1] * in.y + m_ViewMatrix[1][2] * in.z + m_ViewMatrix[1][3]) / z;

    // Prevents rounded corners.
    out = ImTrunc(out);

    return true;
}

void CMath::TransformAABB(const matrix3x4_t& transform, const Vector& minsIn, const Vector& maxsIn, Vector& minsOut, Vector& maxsOut) {
    const Vector localCenter = (minsIn + maxsIn) * 0.5f;
    const Vector localExtent = maxsIn - localCenter;

    const auto& mat = transform.m_flMatVal;
    const Vector worldAxisX{mat[0][0], mat[0][1], mat[0][2]};
    const Vector worldAxisY{mat[1][0], mat[1][1], mat[1][2]};
    const Vector worldAxisZ{mat[2][0], mat[2][1], mat[2][2]};

    const Vector worldCenter = localCenter.Transform(transform);
    const Vector worldExtent{
        localExtent.DotProductAbsolute(worldAxisX),
        localExtent.DotProductAbsolute(worldAxisY),
        localExtent.DotProductAbsolute(worldAxisZ),
    };

    minsOut = worldCenter - worldExtent;
    maxsOut = worldCenter + worldExtent;
}
