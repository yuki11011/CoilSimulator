#pragma once

#include <cmath>

struct Vec2 {
    double x{};
    double y{};

    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }
};

struct Vec3 {
    double x{};
    double y{};
    double z{};

    double magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    double dot(const Vec3& right) const {
        return x * right.x + y * right.y + z * right.z;
    }

    Vec3 cross(const Vec3& right) const {
        return Vec3{
            y * right.z - z * right.y,
            z * right.x - x * right.z,
            x * right.y - y * right.x
        };
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3{ x + other.x, y + other.y, z + other.z };
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3{ x - other.x, y - other.y, z - other.z };
    }

    Vec3 operator*(double scaler) const {
        return Vec3{ scaler * x, scaler * y, scaler * z };
    }
};