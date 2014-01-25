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

#ifndef YAMCR_TEXTURES_TEXTURE_H__
#define YAMCR_TEXTURES_TEXTURE_H__

#include "vector.h"
#include "spectrum.h"
#include <array>

namespace yamcr {

template <int nChannels>
class Texture {
public:
    virtual std::array<float, nChannels> Eval(const Point2 &st) const = 0;
};

typedef Texture<RGBSpectrum::Dimension> TextureSpectrum;

}

#endif //YAMCR_TEXTURES_TEXTURE_H__
