#pragma once

#include <imgui/imgui.h>

struct BBox_t {
    BBox_t() { Invalidate(); }

    void Invalidate() {
        static constexpr auto MAX_FLOAT = std::numeric_limits<float>::max();

        m_Mins.x = m_Mins.y = MAX_FLOAT;
        m_Maxs.x = m_Maxs.y = -MAX_FLOAT;

        std::fill(std::begin(m_Vertices), std::end(m_Vertices), ImVec2{-MAX_FLOAT, -MAX_FLOAT});
    }

    ImVec2 m_Mins, m_Maxs;
    ImVec2 m_Vertices[8];
};
