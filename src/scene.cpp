#include "scene.h"
#include "vector.h"

namespace yamcr {

Scene::Scene(const std::vector<std::shared_ptr<Shape>> &shapes) {
    m_RtcScene = rtcNewScene(
            RTC_SCENE_STATIC, RTC_INTERSECT1);

    for(auto it = shapes.begin(); it != shapes.end(); it++) 
        (*it)->Register(m_RtcScene);

    rtcCommit(m_RtcScene);
}

Scene::~Scene() {
    rtcDeleteScene(m_RtcScene);
}

bool Scene::Intersect(Ray &ray, Intersection *isect) {
    rtcIntersect(m_RtcScene, ray.ToRTCRay());
    if(ray.geomID != -1) {
        isect->p = ray.org + ray.dir * ray.tfar;
        isect->n = ray.Ng;
        isect->rayEpsilon = 1e-3f*ray.tfar;
        return true;
    }
    return false;
}

bool Scene::Occluded(Ray &ray) {
    rtcOccluded(m_RtcScene, ray.ToRTCRay());
    return ray.geomID != -1;
}

}
