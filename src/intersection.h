#ifndef YAMCR_INTERSECTION_H__
#define YAMCR_INTERSECTION_H__

#include "vector.h"
#include "bsdfs/bsdf.h"
#include <memory>

namespace yamcr {

struct Intersection {    
    Point p;
    Normal n;
    std::shared_ptr<BSDF> bsdf;
    float rayEpsilon;
};

}

#endif //#ifndef YAMCR_INTERSECTION_H__
