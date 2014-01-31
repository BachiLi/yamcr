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

#ifndef YAMCR_RAY_DIFFERENTIAL_H__
#define YAMCR_RAY_DIFFERENTIAL_H__

#include "vector.h"

namespace yamcr {

struct RayDifferential {    
    RayDifferential() {
        dPdx = dPdy = dDdx = dDdy = Vector(0.f);
    }
    RayDifferential(const Vector &right, const Vector &up, const Vector &d, 
            const Vector2 &scale);

    void Scale(float scale);
    void Transfer(const Vector &D, const Normal &n, float t);
    void Reflect(const Vector &D, const Normal &n);
    void Refract(const Vector &D, const Normal &n, float eta);

    // For surface texture mapping
    Vector dPdx, dPdy;
    // For environment map background
    Vector dDdx, dDdy;
};

}

#endif //YAMCR_RAY_DIFFERENTIAL_H__
