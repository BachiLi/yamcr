#include "film.h"

#include <OpenImageIO/imagebufalgo.h>

namespace yamcr {

Film::Film(int width, int height) {
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

void Film::Write(const std::string &filename) {        
    OpenImageIO::ImageBuf buf;
    OpenImageIO::ImageBufAlgo::unpremult(buf, *m_Buffer);
    OpenImageIO::ImageBuf rgbBuf;
    OpenImageIO::ImageBufAlgo::channels(rgbBuf, buf, 3, NULL);
    rgbBuf.write(filename);
}

}
