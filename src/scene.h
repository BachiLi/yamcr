#ifndef YAMCR_SCENE_H__
#define YAMCR_SCENE_H__

#include <embree2/rtcore.h>
#include "ray.h"

namespace yamcr {
    
class Scene {
public:
    Scene();    
    ~Scene();

    bool Intersect(Ray &ray);    
private:
    RTCScene m_RtcScene;
};

}

#endif //YAMCR_SCENE_H__
