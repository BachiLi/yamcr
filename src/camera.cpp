#include "camera.h"

namespace yamcr {

Ray Camera::GenerateRay(int x, int y) const {
    float nx = (((float)x+0.5f)/(float)m_XRes) - 0.5f;
    float ny = - ((((float)y+0.5f)/(float)m_YRes) - 0.5f);
    Ray ray(Point(0.f, 0.f, -5.f),
            Normalize(Vector(nx, ny, 1.f)));
    return ray;
}

}
