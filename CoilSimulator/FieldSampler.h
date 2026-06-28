#pragma once

#include "Vector.h"
#include "MagneticFieldModel.h"
#include <vector>

struct FieldSample {
    Vec2 pos{};
    Vec2 field{};
    double magnitude{};
};

struct SamplingDesc {
    double minimumX{ -2.0 };
    double maximumX{ 2.0 };

    double minimumY{ -2.0 };
    double maximumY{ 2.0 };

    int horizontalCount{ 100 }; // …•½•ûŒü‚Ì“_‚ÌŒÂ”
    int verticalCount{ 100 }; // ‚’¼•ûŒü‚Ì“_‚ÌŒÂ”
};

class FieldSampler {
public:
    std::vector<FieldSample> sample(
        const MagneticFieldModel& model,
        const SamplingDesc& desc
    ) const;
};