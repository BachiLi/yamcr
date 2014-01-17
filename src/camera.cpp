#include "camera.h"

namespace yamcr {

Camera::Camera(const Point &pos, const Vector &dir, const Vector &up, 
               int xres, int yres) :
    m_Pos(pos), m_Dir(dir), m_Up(up), m_Right(m_Up.cross(m_Dir)), 
    m_XRes(xres), m_YRes(yres) {
}

Ray Camera::GenerateRay(const Point2 &screenPos) const {
    float nx = (screenPos[0]/(float)m_XRes) - 0.5f;
    float ny = - ((screenPos[1]/(float)m_YRes) - 0.5f);
    Ray ray(m_Pos,
            Vector(nx*m_Right + ny*m_Up + m_Dir).normalized());
    return ray;
}

}
