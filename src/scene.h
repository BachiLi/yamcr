#ifndef YAMCR_SCENE_H__
#define YAMCR_SCENE_H__

#include "ray.h"
#include "intersection.h"
#include "primitive.h"
#include "lights/light.h"
#include <vector>
#include <memory>
#include <embree2/rtcore.h>

namespace yamcr {
    
class Scene {
public:
    typedef std::vector<std::shared_ptr<Light>>::iterator LightIterator;
    typedef std::vector<std::shared_ptr<Light>>::const_iterator ConstLightIterator;

    Scene(const std::vector<std::shared_ptr<Primitive>> &prims,
          const std::vector<std::shared_ptr<Light>> &lights);
    ~Scene();

    bool Intersect(Ray &ray, Intersection *isect);
    bool Occluded(Ray &ray);
   
    LightIterator GetLightIteratorBegin() {return m_Lights.begin();}
    ConstLightIterator GetLightIteratorBegin() const {return m_Lights.begin();}
    LightIterator GetLightIteratorEnd() {return m_Lights.end();}
    ConstLightIterator GetLightIteratorEnd() const {return m_Lights.end();}
private:
    RTCScene m_RtcScene;
    std::vector<std::shared_ptr<Primitive>> m_Primitives;
    std::vector<std::shared_ptr<Light>> m_Lights;
};

}

#endif //YAMCR_SCENE_H__
