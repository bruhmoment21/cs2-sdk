#include "math.hpp"

#include "../memory/memory.hpp"

#include <imgui/imgui.h>

bool math::WorldToScreen(const Vector& in, Vector& out) {
    if (memory::fnScreenTransform(in, out)) return false;

    const ImVec2& screenSize = ImGui::GetIO().DisplaySize;
    out.x = ((out.x + 1.f) * 0.5f) * screenSize.x;
    out.y = screenSize.y - (((out.y + 1.f) * 0.5f) * screenSize.y);

    return true;
}
