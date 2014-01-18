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

void BlockedRenderer::RenderBlock(int taskId) {    
    const int tileY = taskId / m_TilesNumX;
    const int tileX = taskId - tileY * m_TilesNumX;
    const int x0 = tileX * m_BlockSize;
    const int x1 = std::min(x0 + m_BlockSize, m_XRes);
    const int y0 = tileY * m_BlockSize;
    const int y1 = std::min(y0 + m_BlockSize, m_YRes);
    std::shared_ptr<Sampler> localSampler = m_Sampler->Clone();

    for(int y = x0; y < x1; y++) {
        for(int x = y0; x < y1; x++) {           
            for(int s = 0; s < m_Spp; s++) {                
                Ray ray = m_Camera->GenerateRay(Point2(x, y) + localSampler->Next2D());
                Intersection isect;
                RGBSpectrum L(0.f);
                if(m_Scene->Intersect(ray, &isect)) {                   
                    for(auto it = m_Scene->GetLightIteratorBegin(); it != m_Scene->GetLightIteratorEnd(); it++) {
                        Ray shadowRay;
                        RGBSpectrum Le = (*it)->SampleDirect(isect, shadowRay);
                        Normal Ns = isect.Ns;
                        if(ray.dir.dot(Ns) > 0.f)
                            Ns = - Ns;
                        float cos = Ns.dot(shadowRay.dir);
                        if(cos <= 0.f)
                            continue;
                        bool hit = m_Scene->Occluded(shadowRay);
                        if(!hit)        
                            L += Le*isect.bsdf->Eval(-ray.dir, shadowRay.dir)*cos;
                    }
                } 
                m_Film->AddSample(x, y, L);
            }
        }
    }

}

}
