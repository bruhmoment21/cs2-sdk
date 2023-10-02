#pragma once

#include <math/types/matrix3x4.hpp>

class Vector {
   public:
    Vector() : x(0.f), y(0.f), z(0.f) {}
    Vector(float rhs) : x(rhs), y(rhs), z(rhs) {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector operator+(const Vector& rhs) const { return Vector{x + rhs.x, y + rhs.y, z + rhs.z}; }
    Vector operator-(const Vector& rhs) const { return Vector{x - rhs.x, y - rhs.y, z - rhs.z}; }
    Vector operator*(float scalar) const { return Vector{x * scalar, y * scalar, z * scalar}; }

    Vector Transform(const matrix3x4_t& matrix) const {
        return Vector{x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2] + matrix[0][3],
                      x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2] + matrix[1][3],
                      x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2] + matrix[2][3]};
    }

    float DotProductAbsolute(const Vector& rhs) const { return std::abs(x * rhs.x) + std::abs(y * rhs.y) + std::abs(z * rhs.z); }

    Vector Min(const Vector& rhs) const { return Vector{std::min(x, rhs.x), std::min(y, rhs.y), std::min(z, rhs.z)}; }
    Vector Max(const Vector& rhs) const { return Vector{std::max(x, rhs.x), std::max(y, rhs.y), std::max(z, rhs.z)}; }

    float x, y, z;
};
