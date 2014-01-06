#ifndef YAMCR_SAMPLERS_RANDOM_H__
#define YAMCR_SAMPLERS_RANDOM_H__

#include "sampler.h"
#include <random>

namespace yamcr {

class RandomSampler : public Sampler {
public:
    std::shared_ptr<Sampler> Clone();
    float Next1D();
    Point2 Next2D();
private:
    std::mt19937 m_Generator;
    std::uniform_real_distribution<float> m_Dist;
};

}

#endif //YAMCR_SAMPLER_RANDOM_H__
