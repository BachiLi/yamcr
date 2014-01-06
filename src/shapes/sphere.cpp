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
    bounds_o->lower_x = p.x - r;
    bounds_o->lower_y = p.y - r;
    bounds_o->lower_z = p.z - r;
    bounds_o->upper_x = p.x + r;
    bounds_o->upper_y = p.y + r;
    bounds_o->upper_z = p.z + r;
}

void Sphere::IntersectFunc(const Sphere* spheres, RTCRay& rtcRay, size_t item) {
    Ray &ray = Ray::FromRTCRay(rtcRay);
    const Sphere& sphere = spheres[item];
    const Vector v = ray.org - sphere.m_Pos;
    const float A = ray.dir.LengthSquared();
    const float B = 2.0f * Dot(v,ray.dir);
    const float C = v.LengthSquared() - sphere.m_Radius*sphere.m_Radius;
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
    const float A = ray.dir.LengthSquared();
    const float B = 2.0f*Dot(v,ray.dir);
    const float C = v.LengthSquared() - sphere.m_Radius*sphere.m_Radius;
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

}
