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

#ifndef YAMCR_LIGHT_POINT_H__
#define YAMCR_LIGHT_POINT_H__

#include "light.h"

namespace yamcr {

class PointLight : public Light {
public:
    PointLight(const Point &pos, const RGBSpectrum &intensity) :
        m_Pos(pos), m_Intensity(intensity) {}

    RGBSpectrum SampleDirect(const Intersection &isect, Ray &ray) const;

private:
    Point m_Pos;
    RGBSpectrum m_Intensity;
};

}

#endif //YAMCR_LIGHT_POINT_H__
