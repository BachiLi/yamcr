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

#include "raydifferential.h"

#include "fastmath.h"
#include <cmath>

namespace yamcr {

RayDifferential::RayDifferential(const Vector &right, const Vector &up, const Vector &d, 
        const Vector2 &scale) {
    // Equation (8) of [Igehy 1999]
    dPdx = dPdy = Vector(0.f);
    float norm = d.dot(d);
    float powNorm = std::pow(norm, 1.5f);
    dDdx = scale[0]*(norm*right - d.dot(right)*d)/powNorm;
    dDdy = scale[1]*(norm*up - d.dot(up)*d)/powNorm;
}

void RayDifferential::Scale(float scale) {
    dPdx *= scale;
    dPdy *= scale;
    dDdx *= scale;
    dDdy *= scale;
}

void RayDifferential::Transfer(const Vector &D, const Normal &n, float t) {
    Normal fn = n;
    if(fn.dot(D) > 0.f)
        fn = -fn;
    // Equation (10-12) of [Igehy 1999]
    float rDdotN = Rcp(D.dot(fn));
    float dtdx = -(dPdx+t*dDdx).dot(fn) * rDdotN;
    dPdx = dPdx + t*dDdx+dtdx*D;
    float dtdy = -(dPdy+t*dDdy).dot(fn) * rDdotN;
    dPdy = dPdy + t*dDdy+dtdy*D;
    //dDdx/dDdy remain unchanged
}

void RayDifferential::Reflect(const Vector &D, const Normal &n) {
    // TODO: implement it
}

void RayDifferential::Refract(const Vector &D, const Normal &n, float eta) {
    // TODO: implement it
}

} // namespace yamcr

