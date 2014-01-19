/* 
    This file is part of yamcr, an experimental Monte Carlo renderer

    Copyright (c) 2013-2014 by Tzu-Mao Li

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
