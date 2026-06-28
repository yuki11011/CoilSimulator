#include "MagneticFieldModel.h"
#include <iostream>

#include <cmath>
#include <numbers>

MagneticFieldModel::MagneticFieldModel(CoilParameters paramaters)
    : m_parameters(paramaters) {
}

// x-y平面(z = 0)に原点を中心としてコイルを置く
Vec2 MagneticFieldModel::fieldAt(Vec2 pos) const {
    constexpr double vacuumPermeability{ 4 * std::numbers::pi * 1.0e-7 };

    Vec3 field{};
    const Vec3 observationPoint{ pos.x, 0.0, pos.y };
    const double current{ m_parameters.current };
    const int turns{ m_parameters.turns };
    const double radius{ m_parameters.radius };

    const int stepNum{ m_parameters.integrationSegments };
    const double deltaAngle{ 2.0 * std::numbers::pi / static_cast<double>(stepNum) };
    const double minimumDistance{ m_parameters.wireRadius };

    for (int i{ 0 }; i < stepNum; ++i) {
        const double angle{ (static_cast<double>(i) + 0.5) * deltaAngle }; // 分割点の中点で各値を評価する

        Vec3 sourcePoint{
            radius * std::cos(angle),
            radius * std::sin(angle),
            0.0
        };

        Vec3 r{ observationPoint - sourcePoint };
        const double distance{ r.magnitude() };
        Vec3 ds{ 
            Vec3{ -radius * std::sin(angle), radius * std::cos(angle), 0.0 }
            * deltaAngle
        };

        if (distance < minimumDistance) {
            continue;
        }

        const double coefficient{
            vacuumPermeability
            * current
            * turns
            / (
                4.0
                * std::numbers::pi
                * distance * distance * distance)
        };

        const Vec3 dB{ ds.cross(r) * coefficient };

        field = field + dB;
    }

    return Vec2{ field.x, field.z };
}
