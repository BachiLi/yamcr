#ifndef YAMCR_SCENE_H__
#define YAMCR_SCENE_H__

#include "ray.h"
#include "intersection.h"
#include "shapes/shape.h"
#include <vector>
#include <memory>
#include <embree2/rtcore.h>

namespace yamcr {
    
class Scene {
public:
    Scene(const std::vector<std::shared_ptr<Shape>> &shapes);
    ~Scene();

    bool Intersect(Ray &ray, Intersection *isect);
    bool Occluded(Ray &ray);
private:
    RTCScene m_RtcScene;
};

}

#endif //YAMCR_SCENE_H__
