#include "lambertian.h"

#include <cmath>

namespace yamcr {

RGBSpectrum Lambertian::Eval(const Vector &wi, const Vector &wo) const {
    return m_Kd / (float)M_PI;
}

}
