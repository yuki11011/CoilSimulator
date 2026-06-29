#pragma once

#include "Vector.h"
#include "IntegrationPath.h"
#include "MagneticFieldModel.h"
#include <vector>

struct IntegrationSample {
    Vec2 pos{};

    Vec2 lineElement{};
    Vec2 magneticField{};

    double contribution{};
};

class IntegrationModel {
public:
    explicit IntegrationModel(ClosedPath path);

    double integrate(const MagneticFieldModel& magneticField);

    void setPath(ClosedPath path) {
        m_path = path;
    }

    const ClosedPath& path() const { return m_path; };

    const std::vector<IntegrationSample>& samples() const {
        return m_samples;
    }

    double integration() const {
        return m_integration;
    }

private:
    ClosedPath m_path;
    std::vector<IntegrationSample> m_samples{};
    double m_integration{};
};