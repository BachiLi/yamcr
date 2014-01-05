#include "camera.h"

namespace yamcr {

Camera::Camera(const Point &pos, const Vector &dir, const Vector &up, 
               int xres, int yres) :
    m_Pos(pos), m_Dir(dir), m_Up(up), m_Right(Cross(m_Up, m_Dir)), 
    m_XRes(xres), m_YRes(yres) {
}

Ray Camera::GenerateRay(const Point2 &screenPos) const {
    float nx = (screenPos.x/(float)m_XRes) - 0.5f;
    float ny = - ((screenPos.y/(float)m_YRes) - 0.5f);
    Ray ray(m_Pos,
            Normalize(nx*m_Right + ny*m_Up + m_Dir));
    return ray;
}

}
