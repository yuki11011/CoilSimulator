#pragma once

#include "CoilParameters.h"
#include "FieldSampler.h"
#include "IntegrationModel.h"
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

    void setPathDescriptor(const PathDescriptor& desc) {
        m_pathDescriptor = desc;
        m_integrationModel.setPath(ClosedPath{ desc });
    }

    void rebuild();

    const SamplingDesc& samplingDesc() const {
        return m_samplingDesc;
    };

    const std::vector<FieldSample>& samples() const {
        return m_samples;
    };

    const std::vector<IntegrationSample>& integrationSamples() const {
        return m_integrationModel.samples();
    }

    double integrationValue() const {
        return m_integrationValue;
    }

private:
    CoilParameters m_coilParameters{};
    SamplingDesc m_samplingDesc{};
    PathDescriptor m_pathDescriptor{};

    MagneticFieldModel m_model;
    FieldSampler m_sampler{};
    IntegrationModel m_integrationModel;

    std::vector<FieldSample> m_samples{};
    double m_integrationValue{};
};