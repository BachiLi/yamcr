#ifndef YAMCR_SAMPLER_H__
#define YAMCR_SAMPLER_H__

#include "point.h"

namespace yamcr {

class Sampler {
public:
    virtual float Next1D() = 0;
    virtual Point2 Next2D() = 0;
};

}

#endif //YAMCR_SAMPLER_H__
