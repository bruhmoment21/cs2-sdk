#include "math.hpp"

#include "../memory/memory.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

bool math::WorldToScreen(const Vector& in, Vector& out) {
    if (memory::fnScreenTransform(in, out)) return false;

    const ImVec2 screenHalvedSize = ImGui::GetIO().DisplaySize * 0.5f;

    out.x = screenHalvedSize.x * (1 + out.x);
    out.y = screenHalvedSize.y * (1 - out.y);

    return true;
}
