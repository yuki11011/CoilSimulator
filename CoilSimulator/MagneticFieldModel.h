#pragma once

#include "CoilParameters.h"
#include "Vector.h"

class MagneticFieldModel {
public:
    explicit MagneticFieldModel(CoilParameters parameter);

    Vec2 fieldAt(Vec2 pos) const;

    void setParameters(CoilParameters parameter) {
        m_parameters = parameter;
    }

private:
    CoilParameters m_parameters;
};