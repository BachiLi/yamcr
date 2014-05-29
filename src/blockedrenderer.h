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

#ifndef YAMCR_BLOCKED_RENDERER_H__
#define YAMCR_BLOCKED_RENDERER_H__

#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "film.h"
#include "samplers/sampler.h"
#include <memory>

namespace yamcr {

class BlockedRenderer : public Renderer {
public:
    BlockedRenderer(std::shared_ptr<Scene> scene,
                    std::shared_ptr<Camera> camera,
                    std::shared_ptr<Sampler> sampler,
                    int blockSize, int spp, int numThreads) :
        m_Scene(scene), m_Camera(camera), m_Film(camera->GetFilm()), m_Sampler(sampler),
        m_BlockSize(blockSize), m_Spp(spp), m_NumThreads(numThreads), 
        m_XRes(m_Film->GetXRes()), m_YRes(m_Film->GetYRes()) {
        m_TilesNumX = (m_XRes + blockSize-1) / blockSize;
        m_TilesNumY = (m_YRes + blockSize-1) / blockSize;
    }

    void Render();
private:
    void RenderBlock(int taskId);
    RGBSpectrum LightSampling(const Scene::ConstLightIterator &light,
        const Point2 &lightSample, const Intersection &isect, const Vector &wo, Vector *wi, float *pdf) const;
    RGBSpectrum BSDFSampling(const Scene::ConstLightIterator &light,
        const Point2 &bsdfSample, const Intersection &isect, const Vector &wo, Vector *wi, float *pdf) const;


    std::shared_ptr<Scene> m_Scene;
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<Film> m_Film;
    std::shared_ptr<Sampler> m_Sampler;
    int m_BlockSize;
    int m_Spp;
    int m_NumThreads;

    int m_XRes, m_YRes;
    int m_TilesNumX, m_TilesNumY;    
};

}

#endif //YAMCR_BLOCKED_RENDERER_H__
