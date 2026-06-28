#pragma once

#include "CoilParameters.h"
#include "FieldSampler.h"
#include <vector>

class FieldScene {
public:
    FieldScene();

    void setCoilParameters(const CoilParameters& param) {
        m_coilParameters = param;
        m_model.setParameters(param);
    };

    void setSamplingDesc(const SamplingDesc& desc) {
        m_samplingDesc = desc;
    };

    void rebuild();

    const SamplingDesc& samplingDesc() const {
        return m_samplingDesc;
    };

    const std::vector<FieldSample>& samples() const {
        return m_samples;
    };

private:
    CoilParameters m_coilParameters{};
    SamplingDesc m_samplingDesc{};

    MagneticFieldModel m_model;
    FieldSampler m_sampler{};

    std::vector<FieldSample> m_samples{};
};