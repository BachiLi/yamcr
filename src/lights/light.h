#ifndef YAMCR_LIGHTS_LIGHT_H__
#define YAMCR_LIGHTS_LIGHT_H__

#include "spectrum.h"
#include "ray.h"
#include "intersection.h"

namespace yamcr {

class Light {
public:    
    virtual RGBSpectrum SampleDirect(const Intersection &isect, Ray &ray) const = 0;
};

}

#endif //YAMCR_LIGHTS_LIGHT_H__
