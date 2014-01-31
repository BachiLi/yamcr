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

#ifndef YAMCR_INTERSECTION_H__
#define YAMCR_INTERSECTION_H__

#include "vector.h"
#include "bsdfs/bsdf.h"
#include "raydifferential.h"
#include <memory>

namespace yamcr {

class BSDF;

struct Intersection {    
    void ComputeTextureDifferential(const RayDifferential &rayDiff);

    Point p;
    Point2 uv;
    Point2 st;
    Normal Ng, Ns;
    std::shared_ptr<BSDF> bsdf;
    float rayEpsilon;
    uint32_t geomID, primID;
    Vector dPds, dPdt;

    Vector2 dSTdx, dSTdy;    
};

}

#endif //#ifndef YAMCR_INTERSECTION_H__
