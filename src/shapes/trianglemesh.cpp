#include "trianglemesh.h"

#include <embree2/rtcore.h>
#include <algorithm>
#include <stdexcept>

namespace yamcr {

void TriangleMesh::Register(RTCScene rtcScene, unsigned int geomID) {
    unsigned int obtainedGeomID = rtcNewTriangleMesh(rtcScene, RTC_GEOMETRY_STATIC, 
            triangles.size(), vertices.size(), 1);
    if(obtainedGeomID != geomID)
        throw std::runtime_error("Wrong geomID provided when registering a TriangleMesh");
    PointA* rtcVertices = (PointA*) rtcMapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);
    std::copy(vertices.begin(), vertices.end(), rtcVertices);
    rtcUnmapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);

    Triangle* rtcTriangles = (Triangle*) rtcMapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
    std::copy(triangles.begin(), triangles.end(), rtcTriangles);
    rtcUnmapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
}

}
