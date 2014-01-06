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
