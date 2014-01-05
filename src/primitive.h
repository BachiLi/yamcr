#ifndef YAMCR_PRIMITIVE_H__
#define YAMCR_PRIMITIVE_H__

#include "shapes/shape.h"
#include "bsdfs/bsdf.h"
#include <memory>

namespace yamcr {

struct Primitive {
    Primitive(const std::shared_ptr<Shape> shape,
              const std::shared_ptr<BSDF> bsdf) :
        shape(shape), bsdf(bsdf) {}
    const std::shared_ptr<Shape> shape;
    const std::shared_ptr<BSDF> bsdf;
};

}

#endif //YAMCR_PRIMITIVE_H__
