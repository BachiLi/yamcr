#ifndef YAMCR_SHAPES_SHAPE_H__
#define YAMCR_SHAPES_SHAPE_H__

#include <embree2/rtcore.h>
#include "intersection.h"

namespace yamcr {

class Shape {
public:
    /// Register the shape to embree scene
    virtual void Register(RTCScene scene, unsigned int geomID) = 0;
    /// Fill the rest of the intersection struct
    virtual void PostIntersect(Intersection &isect) const = 0;
};

}

#endif //YAMCR_SHAPES_SHAPE_H__
