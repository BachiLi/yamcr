#include "scene.h"
#include "vector.h"

namespace yamcr {

struct Triangle {int v0, v1, v2;};

Scene::Scene() {
    m_RtcScene = rtcNewScene(
            RTC_SCENE_STATIC, RTC_INTERSECT1);

    unsigned geomID = rtcNewTriangleMesh(m_RtcScene, RTC_GEOMETRY_STATIC, 1, 3, 1);
    PointA* vertices = (PointA*) rtcMapBuffer(m_RtcScene, geomID, RTC_VERTEX_BUFFER);
    vertices[0] = Point( 0.f,  1.f,  1.f);
    vertices[1] = Point(-1.f, -1.f,  1.f);
    vertices[2] = Point( 1.f, -1.f,  1.f);
    rtcUnmapBuffer(m_RtcScene, geomID, RTC_VERTEX_BUFFER);

    Triangle* triangles = (Triangle*) rtcMapBuffer(m_RtcScene, geomID, RTC_INDEX_BUFFER);
    triangles[0] = {.v0 = 0, .v1 = 1, .v2 = 2};
    rtcUnmapBuffer(m_RtcScene, geomID, RTC_INDEX_BUFFER);

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
