#ifndef YAMCR_BLOCKED_RENDERER_H__
#define YAMCR_BLOCKED_RENDERER_H__

#include "scene.h"
#include "camera.h"
#include "film.h"
#include "samplers/sampler.h"
#include <memory>

namespace yamcr {

class BlockedRenderer {
public:
    BlockedRenderer(std::shared_ptr<Scene> scene,
                    std::shared_ptr<Camera> camera,
                    std::shared_ptr<Film> film,
                    std::shared_ptr<Sampler> sampler,
                    int blockSize, int spp, int numThreads) :
        m_Scene(scene), m_Camera(camera), m_Film(film), m_Sampler(sampler),
        m_BlockSize(blockSize), m_Spp(spp), m_NumThreads(numThreads), 
        m_XRes(m_Film->GetXRes()), m_YRes(m_Film->GetYRes()) {
        m_TilesNumX = (m_XRes + blockSize-1) / blockSize;
        m_TilesNumY = (m_YRes + blockSize-1) / blockSize;
    }

    void Render();
private:
    void RenderBlock(int taskId);

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
