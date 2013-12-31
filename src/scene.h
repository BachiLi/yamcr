#ifndef YAMCR_SCENE_H__
#define YAMCR_SCENE_H__

#include "ray.h"
#include "trianglemesh.h"
#include <vector>
#include <memory>
#include <embree2/rtcore.h>

namespace yamcr {
    
class Scene {
public:
    Scene(const std::vector<std::shared_ptr<TriangleMesh>> &shapes);
    ~Scene();

    bool Intersect(Ray &ray);    
private:
    RTCScene m_RtcScene;
};

}

#endif //YAMCR_SCENE_H__
