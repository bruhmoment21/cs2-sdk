#include "math.hpp"

#include "../memory/memory.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

static VMatrix g_viewMatrix;

bool math::WorldToScreen(const Vector& in, ImVec2& out) {
    if (!ImGui::GetCurrentContext()) return false;

    const float z = g_viewMatrix[3][0] * in.x + g_viewMatrix[3][1] * in.y +
                    g_viewMatrix[3][2] * in.z + g_viewMatrix[3][3];
    if (z < 0.001f) return false;

    out = ImGui::GetIO().DisplaySize * 0.5f;
    out.x *= 1.0f + (g_viewMatrix[0][0] * in.x + g_viewMatrix[0][1] * in.y +
                     g_viewMatrix[0][2] * in.z + g_viewMatrix[0][3]) /
                        z;
    out.y *= 1.0f - (g_viewMatrix[1][0] * in.x + g_viewMatrix[1][1] * in.y +
                     g_viewMatrix[1][2] * in.z + g_viewMatrix[1][3]) /
                        z;

    // So 'rounded' corners will not appear.
    out = ImFloor(out);
    return true;
}

void math::UpdateViewMatrix(VMatrix* pViewMatrix) {
    if (!pViewMatrix) return;
    g_viewMatrix = *pViewMatrix;
}
