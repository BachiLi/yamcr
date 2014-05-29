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

#ifndef YAMCR_LIGHTS_LIGHT_H__
#define YAMCR_LIGHTS_LIGHT_H__

#include "spectrum.h"
#include "ray.h"
#include "intersection.h"

namespace yamcr {

class Light {
public:    
    virtual bool IsDeltaLight() const = 0;
    virtual RGBSpectrum EvalDirect(const Vector &wi) const = 0;
    virtual RGBSpectrum SampleDirect(const Point2 &sample,
            const Intersection &isect, Ray &ray, float *pdf) const = 0;
    virtual float SampleDirectPdf(
            const Intersection &isect, const Vector &wi) const = 0;

};

}

#endif //YAMCR_LIGHTS_LIGHT_H__
