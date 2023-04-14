#pragma once

#include "types/vector.hpp"

struct ImVec2;

namespace math {
    bool WorldToScreen(const Vector& in, ImVec2& out);
    void UpdateViewMatrix(VMatrix* pViewMatrix);
}  // namespace math
