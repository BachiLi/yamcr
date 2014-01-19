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

#ifndef YAMCR_FILM_H__
#define YAMCR_FILM_H__

#include <OpenImageIO/imagebuf.h>
#include "spectrum.h"

namespace yamcr {

class Film {
public:
    Film(int width, int height, const std::string &filename);
    ~Film();

    void AddSample(float x, float y, const RGBSpectrum &val);
    void Write();
    int GetXRes() const {
        return m_Buffer->spec().width;
    }
    int GetYRes() const {
        return m_Buffer->spec().height;
    }
private:
    OpenImageIO::ImageBuf *m_Buffer;
    const std::string m_Filename;
};

}

#endif //YAMCR_FILM_H__
