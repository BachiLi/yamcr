#ifndef YAMCR_BSDFS_BSDF_H__
#define YAMCR_BSDFS_BSDF_H__

#include "spectrum.h"
#include "vector.h"

namespace yamcr {

class BSDF {
public:
    virtual RGBSpectrum Eval(const Vector &wi, const Vector &wo) const = 0;
};

}

#endif //YAMCR_BSDFS_BSDF_H__
