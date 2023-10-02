#pragma once

struct matrix3x4_t {
    auto operator[](int i) const { return m_flMatVal[i]; }
    auto operator[](int i) { return m_flMatVal[i]; }

    float m_flMatVal[3][4];
};
