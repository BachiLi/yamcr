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

#ifndef YAMCR_CAMERA_H__
#define YAMCR_CAMERA_H__

#include "ray.h"
#include "vector.h"
#include "raydifferential.h"
#include "film.h"

#include <memory>

namespace yamcr {

class Camera {
public:
    Camera(const Point &pos, const Vector &dir, const Vector &up, 
           std::shared_ptr<Film> film);
    Ray GenerateRay(const Point2 &screenPos, RayDifferential *rayDiff) const;
    std::shared_ptr<Film> GetFilm() const {return m_Film;}
private:
    const Point m_Pos;
    const Vector m_Dir, m_Up, m_Right;
    std::shared_ptr<Film> m_Film;
    float m_InvXRes, m_InvYRes;
};

}

#endif //YAMCR_CAMERA_H__
