#include "IntegrationPath.h"

#include <numbers>
#include <cmath>

ClosedPath::ClosedPath(PathDescriptor desc)
    : m_desc{ desc } {
    makePath();
}

void ClosedPath::makePath() {
    double deltaAngle{ 2 * std::numbers::pi / m_desc.integrationSegments };
    double radius{ m_desc.radius };


    for (int i{ 0 }; i < m_desc.integrationSegments; ++i) {
        m_points.push_back(
            Vec2{
                radius * std::cos(deltaAngle * i) + m_desc.center.x,
                radius * std::sin(deltaAngle * i) + m_desc.center.y
            }
        );
    }
}
