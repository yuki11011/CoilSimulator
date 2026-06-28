#include "FieldScene.h"

FieldScene::FieldScene() 
    : m_model{ m_coilParameters } {
    rebuild();
}

void FieldScene::rebuild() {
    m_samples = m_sampler.sample(
        m_model,
        m_samplingDesc
    );
}
