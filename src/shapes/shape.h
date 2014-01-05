#ifndef YAMCR_SHAPES_SHAPE_H__
#define YAMCR_SHAPES_SHAPE_H__

#include <embree2/rtcore.h>

namespace yamcr {

class Shape {
public:
    virtual void Register(RTCScene scene) const = 0;
};

}

#endif //YAMCR_SHAPES_SHAPE_H__
