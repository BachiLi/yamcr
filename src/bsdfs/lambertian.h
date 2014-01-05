#ifndef YAMCR_BSDFS_LAMBERTIAN_H__
#define YAMCR_BSDFS_LAMBERTIAN_H__

#include "bsdf.h"

namespace yamcr {

class Lambertian : public BSDF {
public:
    Lambertian(const RGBSpectrum &Kd)
        : m_Kd(Kd) {}
    RGBSpectrum Eval(const Vector &wi, const Vector &wo) const;
private:
    RGBSpectrum m_Kd;
};

}

#endif //YAMCR_BSDFS_LAMBERTIAN_H__
