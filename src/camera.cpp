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

#include "camera.h"

namespace yamcr {

Camera::Camera(const Point &pos, const Vector &dir, const Vector &up, 
               int xres, int yres) :
    m_Pos(pos), m_Dir(dir), m_Up(up), m_Right(m_Up.cross(m_Dir)), 
    m_XRes(xres), m_YRes(yres) {
}

Ray Camera::GenerateRay(const Point2 &screenPos, RayDifferential *rayDiff) const {
    float nx = (screenPos[0]/(float)m_XRes) - 0.5f;
    float ny = - ((screenPos[1]/(float)m_YRes) - 0.5f);
    Vector D = Vector(nx*m_Right + ny*m_Up + m_Dir);
    if(rayDiff)
        *rayDiff = RayDifferential(m_Right, m_Up, D, Vector2(1.f/m_XRes, 1.f/m_YRes));
    Ray ray(m_Pos,
            D.normalized());
    return ray;
}

}
