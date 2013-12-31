#include "scene.h"
#include "vector.h"

namespace yamcr {

Scene::Scene(const std::vector<std::shared_ptr<TriangleMesh>> &shapes) {
    m_RtcScene = rtcNewScene(
            RTC_SCENE_STATIC, RTC_INTERSECT1);

    for(auto it = shapes.begin(); it != shapes.end(); it++) 
        (*it)->Register(m_RtcScene);

    rtcCommit(m_RtcScene);
}

Scene::~Scene() {
    rtcDeleteScene(m_RtcScene);
}

bool Scene::Intersect(Ray &ray) {
    rtcIntersect(m_RtcScene, ray.ToRTCRay());
    return ray.geomID != -1;
}

}
