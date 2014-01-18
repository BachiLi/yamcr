#include "point.h"

namespace yamcr {

RGBSpectrum PointLight::SampleDirect(const Intersection &isect, Ray &ray) const {
    ray.org = isect.p;
    Vector v = m_Pos - isect.p;
    float dist = v.norm();
    ray.dir = v / dist;
    ray.tnear = isect.rayEpsilon;
    ray.tfar = dist - isect.rayEpsilon;
    return m_Intensity / (dist*dist);
}

}
