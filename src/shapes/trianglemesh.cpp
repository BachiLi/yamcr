#include "trianglemesh.h"

#include <embree2/rtcore.h>
#include <algorithm>

namespace yamcr {

void TriangleMesh::Register(RTCScene rtcScene) const {
    unsigned geomID = rtcNewTriangleMesh(rtcScene, RTC_GEOMETRY_STATIC, 
            triangles.size(), vertices.size(), 1);
    PointA* rtcVertices = (PointA*) rtcMapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);
    std::copy(vertices.begin(), vertices.end(), rtcVertices);
    rtcUnmapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);

    Triangle* rtcTriangles = (Triangle*) rtcMapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
    std::copy(triangles.begin(), triangles.end(), rtcTriangles);
    rtcUnmapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
}

}
