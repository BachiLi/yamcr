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

#include "point.h"

namespace yamcr {

RGBSpectrum PointLight::SampleDirect(const Intersection &isect, Ray &ray) const {
    ray.org = isect.p;
    Vector v = m_Pos - isect.p;
    float dist = v.norm();
    ray.dir = v / dist;
    ray.tnear = isect.rayEpsilon;
    ray.tfar = dist - isect.rayEpsilon;
    return m_Intensity / (dist*dist);
}

}
