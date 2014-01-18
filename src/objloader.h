#ifndef YAMCR_OBJLOADER_H__
#define YAMCR_OBJLOADER_H__

#include "shapes/trianglemesh.h"
#include "transform.h"

#include <memory>
#include <string>

namespace yamcr {

class ObjLoader {
public:
    ObjLoader(bool computeNormals) :
        m_ComputeNormals(computeNormals) {}
    std::shared_ptr<TriangleMesh> Load(
            const Transform &obj2world, const std::string &filename);
private:
    void ComputeNormals(const std::vector<PointA> &vertices, const std::vector<Triangle> &triangles, 
            std::vector<Normal> &normals);

    bool m_ComputeNormals;
};

}

#endif //YAMCR_OBJLOADER_H__
