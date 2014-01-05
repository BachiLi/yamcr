#ifndef YAMCR_POINTLIGHT_H__
#define YAMCR_POINTLIGHT_H__

#include "light.h"

namespace yamcr {

class PointLight : public Light {
public:
    PointLight(const Point &pos, const RGBSpectrum &intensity) :
        m_Pos(pos), m_Intensity(intensity) {}

    RGBSpectrum SampleDirect(const Intersection &isect, Ray &ray) const;

private:
    Point m_Pos;
    RGBSpectrum m_Intensity;
};

}

#endif //YAMCR_POINTLIGHT_H__
