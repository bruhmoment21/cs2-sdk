#pragma once

#include <math/types/quaternion.hpp>
#include <math/types/vector.hpp>

class alignas(16) CTransform {
   public:
    alignas(16) Vector m_Position;
    alignas(16) Quaternion m_Orientation;

    matrix3x4_t ToMatrix() const;
};
