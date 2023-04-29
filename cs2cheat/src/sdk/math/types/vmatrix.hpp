#pragma once

class VMatrix {
   public:
    auto operator[](int i) const noexcept { return m[i]; }

    float m[4][4];
};
