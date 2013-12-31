#ifndef YAMCR_TRIANGLEMESH_H__
#define YAMCR_TRIANGLEMESH_H__

#include "point.h"
#include <vector>
#include <embree2/rtcore.h>

namespace yamcr {

struct Triangle {
    Triangle(uint32_t id0, uint32_t id1, uint32_t id2) {
        idx[0] = id0;
        idx[1] = id1;
        idx[2] = id2;
    }
    // Indices to vertex buffer    
    uint32_t idx[3];    
};

class TriangleMesh {
public:
    TriangleMesh(const std::vector<PointA> &vertices,
                 const std::vector<Triangle> &triangles) :
        vertices(vertices), triangles(triangles) {}

    void Register(RTCScene rtcScene) const;
private:
    std::vector<PointA> vertices;
    std::vector<Triangle> triangles;
};

}

#endif //YAMCR_TRIANGLEMESH_H__
