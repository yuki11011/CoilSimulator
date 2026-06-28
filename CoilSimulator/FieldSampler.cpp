#include "FieldSampler.h"

#include <cstddef>

std::vector<FieldSample> FieldSampler::sample(const MagneticFieldModel& model, const SamplingDesc& desc) const {
    const std::size_t totalCount{
        static_cast<std::size_t>(desc.horizontalCount) * static_cast<std::size_t>(desc.verticalCount) 
    };
    std::vector<FieldSample> samples{};
    samples.reserve(totalCount);

    const double deltaX{ (desc.maximumX - desc.minimumX) / (desc.horizontalCount - 1) };
    const double deltaY{ (desc.maximumY - desc.minimumY) / (desc.verticalCount - 1) };

    for (int xIdx{ 0 }; xIdx < desc.horizontalCount; ++xIdx) {
        for (int yIdx{ 0 }; yIdx < desc.verticalCount; ++yIdx) {
            const Vec2 pos{ desc.minimumX + xIdx * deltaX, desc.minimumY + yIdx *deltaY };
            const Vec2 field{ model.fieldAt(pos) };

            samples.push_back(FieldSample{
                pos,
                field,
                field.magnitude()
                });
        }
    }

    return samples;
}
