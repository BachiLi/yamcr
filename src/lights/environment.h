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

#ifndef YAMCR_LIGHT_ENVIRONMENT_H__
#define YAMCR_LIGHT_ENVIRONMENT_H__

#include "light.h"
#include "distribution.h"
#include "texturesystem.h"

namespace yamcr {

class EnvironmentLight : public Light {
public:
    EnvironmentLight(const std::string &filename);
    ~EnvironmentLight();

    bool IsDeltaLight() const {return true;}
    RGBSpectrum EvalDirect(const Vector &wi) const;
    RGBSpectrum SampleDirect(const Point2 &sample, 
            const Intersection &isect, Ray &ray, float *pdf) const;
    float SampleDirectPdf(
            const Intersection &isect, const Vector &wi) const;
private:
    OpenImageIO::ustring m_Filename;
    // Dealing with weird non-constant declaration
    mutable OpenImageIO::TextureOptions m_Options;
    PiecewiseConstant2D *m_Distribution;
};

}

#endif //YAMCR_LIGHT_POINT_H__
