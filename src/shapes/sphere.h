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
