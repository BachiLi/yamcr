#ifndef YAMCR_RAY_H__
#define YAMCR_RAY_H__

#include "commondefs.h"
#include <embree2/rtcore.h>
#include <embree2/rtcore_ray.h>
#include "vector.h"
#include <limits>

namespace yamcr {

/**
 *  An embree compatible ray
 */
struct Ray {
    Ray() :
        mask(-1), geomID(-1), primID(-1), instID(-1) {}
    Ray(const Point &org, const Vector &dir, 
            float tnear=0.f, float tfar=std::numeric_limits<float>::infinity(),
            float time=0.f, int mask=-1) :
        org(org), dir(dir), tnear(tnear), tfar(tfar), time(time), mask(mask),
        geomID(-1), primID(-1), instID(-1)  {}

    static Ray& FromRTCRay(RTCRay &rtcRay) {
        return (Ray&)(rtcRay);
    }
    RTCRay& ToRTCRay() const {
        return (RTCRay&)(*this);
    }

    // Ray data
    Point org;
    int pad0;
    Vector dir;
    int pad1;
    float tnear;
    float tfar;
    float time;
    int32 mask;

    // Hit data
    Normal Ng;
    int pad2;
    float u;
    float v;
    int32 geomID;
    int32 primID;
    int32 instID;
};
 
}

#endif //YAMCR_RAY_H__
