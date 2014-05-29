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

#include "blockedrenderer.h"
#include "ray.h"
#include "intersection.h"

#include <vector>
#include <thread>

#include <ThreadPool.h>

#include <fstream>

namespace yamcr {

void BlockedRenderer::Render() {   
    ThreadPool pool(m_NumThreads);
    std::vector<std::future<void>> results;    
    const int numTasks = m_TilesNumX * m_TilesNumY;
    results.reserve(numTasks);
    for(int i = 0; i < numTasks; i++) 
        results.push_back(pool.enqueue(&BlockedRenderer::RenderBlock, this, i));    

    for(auto &res: results) 
        res.get();

    m_Film->Write();
}

RGBSpectrum BlockedRenderer::LightSampling(
        const Scene::ConstLightIterator &light, const Point2 &lightSample,
        const Intersection &isect, const Vector &wo, Vector *wi, float *pdf) const {
    Ray shadowRay;
    RGBSpectrum Le = (*light)->SampleDirect(lightSample, isect, shadowRay, pdf);
    *wi = shadowRay.dir;
    if(Le.IsBlack() || *pdf == 0.f)
        return RGBSpectrum(0.f);
    Normal Ns = isect.Ns;
    float cos = Ns.dot(shadowRay.dir);
    if(cos <= 0.f)
        return RGBSpectrum(0.f);
    bool hit = m_Scene->Occluded(shadowRay);
    if(hit) 
        return RGBSpectrum(0.f);    
    return Le*isect.bsdf->Eval(isect, wo, shadowRay.dir)*cos / *pdf;    
}

RGBSpectrum BlockedRenderer::BSDFSampling(const Scene::ConstLightIterator &light,
        const Point2 &bsdfSample, const Intersection &isect, const Vector &wo, 
        Vector *wi, float *pdf) const {    

    RGBSpectrum w = isect.bsdf->Sample(bsdfSample, isect, wo, wi, pdf);
    Normal Ns = isect.Ns;
    float cos = Ns.dot(*wi);
    if(cos <= 0.f)
        return RGBSpectrum(0.f);

    Ray ray(isect.p, *wi, isect.rayEpsilon, std::numeric_limits<float>::infinity(), isect.time);
    Intersection newIsect;
    RGBSpectrum L(0.f);
    if(m_Scene->Intersect(ray, &newIsect)) {
        // TODO: add area light contribution
    } else {
        L += (*light)->EvalDirect(*wi);
    }    

    return w * L * cos / *pdf;
}

void BlockedRenderer::RenderBlock(int taskId) {    
    const int tileY = taskId / m_TilesNumX;
    const int tileX = taskId - tileY * m_TilesNumX;
    const int x0 = tileX * m_BlockSize;
    const int x1 = std::min(x0 + m_BlockSize, m_XRes);
    const int y0 = tileY * m_BlockSize;
    const int y1 = std::min(y0 + m_BlockSize, m_YRes);
    std::shared_ptr<Sampler> localSampler = m_Sampler->Clone();

    for(int y = y0; y < y1; y++) {
        for(int x = x0; x < x1; x++) {           
            for(int s = 0; s < m_Spp; s++) {                
                RayDifferential rayDiff;
                Vector2 offset = localSampler->Next2D();
                Ray ray = m_Camera->GenerateRay(Point2(x, y) + offset, &rayDiff);
                rayDiff.Scale(1.f/std::sqrt(m_Spp));
                Intersection isect;
                RGBSpectrum L(0.f);                
                if(m_Scene->Intersect(ray, &isect, &rayDiff)) {                    
                    for(auto light = m_Scene->GetLightIteratorBegin(); 
                             light != m_Scene->GetLightIteratorEnd(); light++) {
                        Vector wo = -ray.dir;
                        float lightPdf = 0.f;
                        Point2 lightSample = localSampler->Next2D();
                        Vector wi;
                        RGBSpectrum lightContrib = 
                            LightSampling(light, lightSample, isect, wo, &wi, &lightPdf);
                        float lightWeight = lightPdf / (lightPdf + isect.bsdf->SamplePdf(isect, wo, wi));
                        L += lightWeight * lightContrib;

                        float bsdfPdf = 0.f;
                        Point2 bsdfSample = localSampler->Next2D();
                        RGBSpectrum bsdfContrib = BSDFSampling(light, bsdfSample, isect, wo, &wi, &bsdfPdf);
                        float bsdfWeight = bsdfPdf / (bsdfPdf + (*light)->SampleDirectPdf(isect, wi));
                        L += bsdfWeight * bsdfContrib;
                    }
                } 
                m_Film->AddSample(x, y, L);
            }
            localSampler->NextSequence();
        }
    }

}

}
