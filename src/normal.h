#ifndef YAMCR_NORMAL_H__
#define YAMCR_NORMAL_H__

#include "vector.h"

namespace yamcr {

/**
 *  A 3-d normal vector borrowed from mitsuba
 *  http://www.mitsuba-renderer.org/
 */
struct Normal : public Vector {
    Normal() {}
    Normal(float x, float y, float z) : Vector(x, y, z) {}
    explicit Normal(float v) : Vector(v) {}
    Normal(const Vector &v) : Vector(v.x, v.y, v.z) {}
    Normal& operator=(const Vector &v) {
        x = v.x; y = v.y; z = v.z;
    }
};

}

#endif //YAMCR_NORMAL_H__
