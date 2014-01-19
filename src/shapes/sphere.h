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

#ifndef YAMCR_SHAPES_SPHERE_H__
#define YAMCR_SHAPES_SPHERE_H__

#include "shape.h"
#include "vector.h"

namespace yamcr {

class Sphere : public Shape {
public:
    Sphere(const Point &p, float r) :
        m_Pos(p), m_Radius(r), m_GeomID(-1) {}

    void Register(RTCScene scene, unsigned int geomID);
    void PostIntersect(Intersection &isect) const;

    static void BoundsFunc(const Sphere *spheres, size_t items, RTCBounds *bounds_o);
    static void IntersectFunc(const Sphere *spheres, RTCRay &rtcRay, size_t items);
    static void OccludedFunc(const Sphere *spheres, RTCRay &rtcRay, size_t items);    
private:
    Point m_Pos;    
    float m_Radius;
    unsigned int m_GeomID;
};

}

#endif //YAMCR_SHAPES_SPHERE_H__
