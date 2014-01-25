/* 
    This file is part of yamcr, an experimental Monte Carlo renderer

    Copyright (c) 2013-2014 by Tzu-Mao Li

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "scene.h"
#include "vector.h"

namespace yamcr {

Scene::Scene(const std::vector<std::shared_ptr<Primitive>> &prims,
             const std::vector<std::shared_ptr<Light>> &lights) :
    m_Primitives(prims), m_Lights(lights) {
    m_RtcScene = rtcNewScene(
            RTC_SCENE_STATIC, RTC_INTERSECT1);

    unsigned int geomID = 0;
    for(auto it = m_Primitives.begin(); it != m_Primitives.end(); it++) 
        (*it)->shape->Register(m_RtcScene, geomID++);

    rtcCommit(m_RtcScene);
}

Scene::~Scene() {
    rtcDeleteScene(m_RtcScene);
}

bool Scene::Intersect(Ray &ray, Intersection *isect) {
    rtcIntersect(m_RtcScene, ray.ToRTCRay());
    if(ray.geomID != -1) {
        isect->p = ray.org + ray.dir * ray.tfar;
        isect->uv = Point2(ray.u, ray.v);
        isect->Ng = ray.Ng.normalized();       
        isect->bsdf = m_Primitives[ray.geomID]->bsdf;
        isect->rayEpsilon = 1e-2f*ray.tfar; 
        isect->geomID = ray.geomID;
        isect->primID = ray.primID;
        m_Primitives[ray.geomID]->shape->PostIntersect(*isect);
        return true;
    }
    return false;
}

bool Scene::Occluded(Ray &ray) {
    rtcOccluded(m_RtcScene, ray.ToRTCRay());
    return ray.geomID != -1;
}

}
