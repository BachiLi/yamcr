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


#ifndef YAMCR_SAMPLING_UTILS_H__
#define YAMCR_SAMPLING_UTILS_H__

#include "vector.h"

namespace yamcr {

Vector CosineHemisphereSampling(const Point2 &uv, float *pdf) {
    float phi = 2.f * M_PI * uv[0];
    float cosTheta = std::sqrt(uv[1]), sinTheta = std::sqrt(1.f - uv[1]);
    Vector dir(std::cos(phi) * sinTheta,
               std::sin(phi) * sinTheta,
               cosTheta);
    *pdf = cosTheta * (1.f / M_PI);
    return dir;
}

Vector CosineHemisphereSampling(const Point2 &uv, const Normal &n, float *pdf) {
    Vector dir = CosineHemisphereSampling(uv, pdf);
    Vector s, t;
    CoordinateSystem(n, s, t);
    dir = Vector(s[0]*dir[0] + t[0]*dir[1] + n[0]*dir[2],
                 s[1]*dir[0] + t[1]*dir[1] + n[1]*dir[2],
                 s[2]*dir[0] + t[2]*dir[1] + n[2]*dir[2]);    
    return dir;
}

float CosineHemisphereSamplingPdf(const Vector &dir, const Normal &n) {
    float dot = dir.dot(n);
    return dot < 0.f ? 0.f : dot * (1.f / M_PI);
}

}

#endif //YAMCR_SAMPLING_UTILS_H__
