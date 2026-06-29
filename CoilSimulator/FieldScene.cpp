#include "FieldScene.h"

FieldScene::FieldScene()
    : m_model{ m_coilParameters },
      m_integrationModel{ ClosedPath{ m_pathDescriptor } } {
    rebuild();
}

void FieldScene::rebuild() {
    m_samples = m_sampler.sample(
        m_model,
        m_samplingDesc
    );

    m_integrationValue = m_integrationModel.integrate(m_model);
}
