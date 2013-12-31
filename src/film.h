#ifndef YAMCR_FILM_H__
#define YAMCR_FILM_H__

#include <OpenImageIO/imagebuf.h>
#include "spectrum.h"

namespace yamcr {

class Film {
public:
    Film(int width, int height);
    ~Film();

    void AddSample(float x, float y, const RGBSpectrum &val);
    void Write(const std::string &filename);
private:
    OpenImageIO::ImageBuf *m_Buffer;
};

}

#endif //YAMCR_FILM_H__
