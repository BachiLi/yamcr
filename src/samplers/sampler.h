#ifndef YAMCR_SAMPLER_H__
#define YAMCR_SAMPLER_H__

#include "point.h"
#include <memory>

namespace yamcr {

class Sampler {
public:
    virtual std::shared_ptr<Sampler> Clone() = 0;
    virtual float Next1D() = 0;
    virtual Point2 Next2D() = 0;
};

}

#endif //YAMCR_SAMPLER_H__
