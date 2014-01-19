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

#include "sphere.h"

#include "vector.h"
#include "fastmath.h"
#include "ray.h"
#include <cmath>
#include <stdexcept>

namespace yamcr {

void Sphere::BoundsFunc(const Sphere* spheres, size_t item, RTCBounds* bounds_o) {
    const Sphere& sphere = spheres[item];
    Point p = sphere.m_Pos;
    float r = sphere.m_Radius;
    bounds_o->lower_x = p[0] - r;
    bounds_o->lower_y = p[1] - r;
    bounds_o->lower_z = p[2] - r;
    bounds_o->upper_x = p[0] + r;
    bounds_o->upper_y = p[1] + r;
    bounds_o->upper_z = p[2] + r;
}

void Sphere::IntersectFunc(const Sphere* spheres, RTCRay& rtcRay, size_t item) {
    Ray &ray = Ray::FromRTCRay(rtcRay);
    const Sphere& sphere = spheres[item];
    const Vector v = ray.org - sphere.m_Pos;
    const float A = ray.dir.squaredNorm();
    const float B = 2.0f * v.dot(ray.dir);
    const float C = v.squaredNorm() - sphere.m_Radius*sphere.m_Radius;
    const float D = B*B - 4.0f*A*C;
    if (D < 0.0f) 
        return;
    const float Q = sqrtf(D);
    const float rcpA = Rcp(A);
    const float t0 = 0.5f*rcpA*(-B-Q);
    const float t1 = 0.5f*rcpA*(-B+Q);
    if ((ray.tnear < t0) & (t0 < ray.tfar)) {
        ray.u = 0.0f;
        ray.v = 0.0f;
        ray.tfar = t0;
        ray.geomID = sphere.m_GeomID;
        ray.primID = item;
        ray.Ng = ray.org + t0*ray.dir - sphere.m_Pos;
    }
    if ((ray.tnear < t1) & (t1 < ray.tfar)) {
        ray.u = 0.0f;
        ray.v = 0.0f;
        ray.tfar = t1;
        ray.geomID = sphere.m_GeomID;
        ray.primID = item;
        ray.Ng = ray.org + t1*ray.dir - sphere.m_Pos;
    }
}

void Sphere::OccludedFunc(const Sphere* spheres, RTCRay& rtcRay, size_t item) {
    Ray &ray = Ray::FromRTCRay(rtcRay);
    const Sphere& sphere = spheres[item];
    const Vector v = ray.org - sphere.m_Pos;
    const float A = ray.dir.squaredNorm();
    const float B = 2.0f*v.dot(ray.dir);
    const float C = v.squaredNorm() - sphere.m_Radius*sphere.m_Radius;
    const float D = B*B - 4.0f*A*C;
    if (D < 0.0f) return;
    const float Q = sqrtf(D);
    const float rcpA = Rcp(A);
    const float t0 = 0.5f*rcpA*(-B-Q);
    const float t1 = 0.5f*rcpA*(-B+Q);
    if ((ray.tnear < t0) & (t0 < ray.tfar)) {
        ray.geomID = 0;
    }
    if ((ray.tnear < t1) & (t1 < ray.tfar)) {
        ray.geomID = 0;
    }
}

void Sphere::Register(RTCScene scene, unsigned int geomID) {
    unsigned int obtainedGeomID = rtcNewUserGeometry(scene, 1);
    if(obtainedGeomID != geomID)
        throw std::runtime_error("Wrong geomID provided when registering a Sphere");
    m_GeomID = geomID;
    rtcSetUserData(scene, geomID, this);
    rtcSetBoundsFunction(scene, geomID, (RTCBoundsFunc)&BoundsFunc);
    rtcSetIntersectFunction(scene, geomID, (RTCIntersectFunc)&IntersectFunc);
    rtcSetOccludedFunction (scene, geomID, (RTCOccludedFunc )&OccludedFunc);
}

void Sphere::PostIntersect(Intersection &isect) const {
    isect.Ns = isect.Ng;
}

}
