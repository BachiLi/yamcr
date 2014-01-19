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

#include "film.h"

#include <OpenImageIO/imagebufalgo.h>

namespace yamcr {

Film::Film(int width, int height, const std::string &filename) :
    m_Filename(filename) {
    OpenImageIO::ImageSpec spec(width, height, 4, 
            OpenImageIO::TypeDesc::FLOAT);
    m_Buffer = new OpenImageIO::ImageBuf(spec);
    OpenImageIO::ImageBufAlgo::zero(*m_Buffer);
}

Film::~Film() {
    delete m_Buffer;
}

void Film::AddSample(float x, float y, const RGBSpectrum &val) {
    float w = 1.f;
    std::array<float, 4> oldVal;
    m_Buffer->getpixel((int)x, (int)y, oldVal.data());
    std::array<float, 4> newVal;
    for(int i = 0; i < 3; i++)
        newVal[i] = oldVal[i] + w*val.data[i];
    newVal[3] += oldVal[3] + w;
    m_Buffer->setpixel((int)x, (int)y, newVal.data());
}

void Film::Write() {        
    OpenImageIO::ImageBuf buf;
    OpenImageIO::ImageBufAlgo::unpremult(buf, *m_Buffer);
    OpenImageIO::ImageBuf rgbBuf;
    OpenImageIO::ImageBufAlgo::channels(rgbBuf, buf, 3, NULL);
    rgbBuf.write(m_Filename);
}

}
