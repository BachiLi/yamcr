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

#include "lambertian.h"
#include "sampling_utils.h"

#include <cmath>

namespace yamcr {

RGBSpectrum Lambertian::Eval(
        const Intersection &isect,
        const Vector &wi, const Vector &wo) const {
    return RGBSpectrum(m_Kd->Eval(isect.st, isect.dSTdx, isect.dSTdy)) / (float)M_PI;
}

RGBSpectrum Lambertian::Sample(
            const Point2 &sample,
            const Intersection &isect,
            const Vector &wo, Vector *wi, float *pdf) const {
    *wi = CosineHemisphereSampling(sample, isect.Ns, pdf);
    return RGBSpectrum(m_Kd->Eval(isect.st, isect.dSTdx, isect.dSTdy)) / (float)M_PI;
}

float Lambertian::SamplePdf(
        const Intersection &isect,
        const Vector &wo, const Vector &wi) const {
    return CosineHemisphereSamplingPdf(wi, isect.Ns);
}

}
