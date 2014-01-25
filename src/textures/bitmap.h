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

#ifndef YAMCR_TEXTURES_BITMAP_H__
#define YAMCR_TEXTURES_BITMAP_H__

#include "texture.h"
#include <array>
#include <OpenImageIO/texture.h>
#include <stdexcept>

namespace yamcr {


class BitmapTextureSystem {
public:
    static OpenImageIO::TextureSystem *s_TextureSystem;
    //friend class BitmapTexture;
};

template <int nChannels> 
class BitmapTexture : public Texture<nChannels> {
public:
    BitmapTexture(const std::string &filename, 
            const Vector2 &stScaler = Vector2(1.f, 1.f));
    std::array<float, nChannels> Eval(const Point2 &uv) const;
private:
    OpenImageIO::ustring m_Filename;
    // Dealing with weird non-constant declaration
    mutable OpenImageIO::TextureOptions m_Options;
    Vector2 m_STScaler;
};

template<int nChannels>
BitmapTexture<nChannels>::BitmapTexture(const std::string &filename, const Vector2 &stScaler) {
    m_Filename = OpenImageIO::ustring(filename);
    m_Options.nchannels = nChannels;
    m_STScaler = stScaler;
}

template<int nChannels>
std::array<float, nChannels> BitmapTexture<nChannels>::Eval(const Point2 &st) const {
    Point2 sST = m_STScaler.array()*st.array();
    std::array<float, nChannels> result;
    if(!BitmapTextureSystem::s_TextureSystem->texture(m_Filename, m_Options, sST[0], sST[1], 
                0.f, 0.f, 0.f, 0.f, result.data()))
        throw std::runtime_error("Error during texture lookup");
    return result;
}

typedef BitmapTexture<RGBSpectrum::Dimension> BitmapTextureSpectrum;

}

#endif //YAMCR_TEXTURES_BITMAP_H__
