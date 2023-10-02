#pragma once

class VMatrix {
   public:
    auto operator[](int i) const { return m[i]; }

    float m[4][4];
};
