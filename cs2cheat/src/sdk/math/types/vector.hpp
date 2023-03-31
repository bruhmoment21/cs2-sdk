#pragma once

class Vector {
   public:
    inline Vector operator+(const Vector& rhs) const noexcept {
        return Vector{this->x + rhs.x, this->y + rhs.y, this->z + rhs.z};
    }

    inline Vector operator-(const Vector& rhs) const noexcept {
        return Vector{this->x - rhs.x, this->y - rhs.y, this->z - rhs.z};
    }

    inline Vector operator*(const float rhs) const noexcept {
        return Vector{this->x * rhs, this->y * rhs, this->z * rhs};
    }

    inline Vector operator/(const float rhs) const noexcept {
        return Vector{this->x / rhs, this->y / rhs, this->z / rhs};
    }

    float x, y, z;
};
