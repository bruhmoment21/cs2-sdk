#pragma once

#include "vmatrix.hpp"

class Vector {
   public:
    Vector operator+(const Vector& rhs) const noexcept {
        return Vector{this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
    }

    Vector operator-(const Vector& rhs) const noexcept {
        return Vector{this->x - rhs.x, this->y - rhs.y, this->z - rhs.z};
    }

    Vector operator*(const float rhs) const noexcept {
        return Vector{this->x * rhs, this->y * rhs, this->z * rhs};
    }

    Vector operator/(const float rhs) const noexcept {
        return Vector{this->x / rhs, this->y / rhs, this->z / rhs};
    }

    float DistToSquared(const Vector& rhs) const noexcept {
        const float _x = (this->x - rhs.x);
        const float _y = (this->y - rhs.y);
        const float _z = (this->z - rhs.z);

        return _x * _x + _y * _y + _z * _z;
    }

    float DistToSquaredInMeters(const Vector& rhs) const noexcept {
        // https://developer.valvesoftware.com/wiki/Dimensions_(Half-Life_2_and_Counter-Strike:_Source)/en
        // 1 foot = 12 units => 1 unit = 0.0254 meters.

        return DistToSquared(rhs) * 0.00064516f;
    }

    float x, y, z;
};
