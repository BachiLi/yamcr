#include "pointlight.h"

namespace yamcr {

RGBSpectrum PointLight::SampleDirect(const Point &p, Ray &ray) const {
    ray.org = p;
    ray.dir = m_Pos - p;
    float dist = ray.dir.Length();
    ray.tfar = dist;
    return m_Intensity / (dist*dist);
}

}
