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


#ifndef YAMCR_TEXTURE_SYSTEM_H__
#define YAMCR_TEXTURE_SYSTEM_H__

#include <OpenImageIO/texture.h>

namespace yamcr {

/**
 *   A wrapper class to oiio's texture system
 *   this might not be the best practice, but i cannot figure out better one for now
 */
class TextureSystem {
public:
    static void Init() {
        s_TextureSystem = OpenImageIO::TextureSystem::create();
        s_TextureSystem->attribute("autotile", 64);
        s_TextureSystem->attribute("automip", 1);
        s_TextureSystem->attribute("forcefloat", 1);
    }
    static void Destroy() {
        OpenImageIO::TextureSystem::destroy(s_TextureSystem);
    }
private:
    static OpenImageIO::TextureSystem *s_TextureSystem;
    template<int nChannels> friend class BitmapTexture;
    friend class EnvironmentLight;
};



}

#endif //YAMCR_TEXTURE_SYSTEM_H__
