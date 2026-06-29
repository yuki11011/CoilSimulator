#include "IntegrationModel.h"

#include <cmath>
#include <numbers>

IntegrationModel::IntegrationModel(ClosedPath path)
    : m_path{ path } {
}

double IntegrationModel::integrate(const MagneticFieldModel& magneticField) {
    m_integration = 0.0;
    m_samples.clear();

    int n{ m_path.desc().integrationSegments };
    double deltaAngle{ 2 * std::numbers::pi / static_cast<double>(n)};

    for (int i{ 0 }; i < n; ++i) {
        Vec2 start{ m_path.points()[i].x, m_path.points()[i].y };
        Vec2 end{ m_path.points()[(i + 1) % n].x, m_path.points()[(i + 1) % n].y };

        Vec2 ds{ end - start };

        Vec2 midPoint{ (start + end) * 0.5 };
        Vec2 B{ magneticField.fieldAt(midPoint) };

        m_samples.push_back(
            IntegrationSample{
                m_path.points()[i],
                ds,
                B,
                B.dot(ds)
            }
        );
        m_integration += B.dot(ds);
    }

    return m_integration;
}
