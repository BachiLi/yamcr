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

#ifndef YAMCR_BSDFS_LAMBERTIAN_H__
#define YAMCR_BSDFS_LAMBERTIAN_H__

#include "bsdf.h"
#include "textures/texture.h"
#include <memory>

namespace yamcr {

class Lambertian : public BSDF {
public:
    Lambertian(const std::shared_ptr<TextureSpectrum> &Kd)
        : m_Kd(Kd) {}
    RGBSpectrum Eval(
            const Intersection &isect,
            const Vector &wi, const Vector &wo) const;
private:
    std::shared_ptr<TextureSpectrum> m_Kd;
};

}

#endif //YAMCR_BSDFS_LAMBERTIAN_H__
