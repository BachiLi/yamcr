#include "random.h"

namespace yamcr {

std::shared_ptr<Sampler> RandomSampler::Clone() {
    std::shared_ptr<RandomSampler> newSampler = 
        std::make_shared<RandomSampler>();
    newSampler->m_Generator = m_Generator;
    newSampler->m_Generator.seed(m_Generator());
    return std::shared_ptr<Sampler>(newSampler);
}

float RandomSampler::Next1D() {
    return m_Dist(m_Generator);
}

Point2 RandomSampler::Next2D() {
    return Point2(m_Dist(m_Generator), m_Dist(m_Generator));
}

}
