#ifndef YAMCR_CAMERA_H__
#define YAMCR_CAMERA_H__

#include "ray.h"

namespace yamcr {

class Camera {
public:
    Camera(int xres, int yres) :
        m_XRes(xres), m_YRes(yres) {}
    Ray GenerateRay(int x, int y) const;
private:
    const int m_XRes, m_YRes;
};

}

#endif //YAMCR_CAMERA_H__
