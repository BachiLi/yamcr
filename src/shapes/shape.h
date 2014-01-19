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

#ifndef YAMCR_SHAPES_SHAPE_H__
#define YAMCR_SHAPES_SHAPE_H__

#include <embree2/rtcore.h>
#include "intersection.h"

namespace yamcr {

class Shape {
public:
    /// Register the shape to embree scene
    virtual void Register(RTCScene scene, unsigned int geomID) = 0;
    /// Fill the rest of the intersection struct
    virtual void PostIntersect(Intersection &isect) const = 0;
};

}

#endif //YAMCR_SHAPES_SHAPE_H__
