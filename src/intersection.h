#ifndef YAMCR_INTERSECTION_H__
#define YAMCR_INTERSECTION_H__

#include "vector.h"
#include "bsdfs/bsdf.h"
#include <memory>

namespace yamcr {

struct Intersection {    
    Point p;
    float u, v;
    Normal Ng, Ns;
    std::shared_ptr<BSDF> bsdf;
    float rayEpsilon;
    uint32_t geomID, primID;
};

}

#endif //#ifndef YAMCR_INTERSECTION_H__
