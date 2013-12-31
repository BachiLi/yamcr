#include "commondefs.h"
#include <OpenImageIO/imageio.h>
#include <stdint.h>
#include <embree2/rtcore.h>
#include <limits>

#include "ray.h"
#include "scene.h"
#include "camera.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512;
const int c_Channels = 3;
const char *c_Filename = "foo.exr";

int main(int argc, char *argv[]) {
    rtcInit(NULL);
    Scene scene;
    Camera camera(c_XRes, c_YRes);
    float pixels[c_XRes*c_YRes*c_Channels] = {0.f};
    float *pixPtr = pixels;
    for(int y = 0; y < c_YRes; y++)
        for(int x = 0; x < c_XRes; x++) {
            Ray ray = camera.GenerateRay(x, y);
            if(scene.Intersect(ray)) {
                *pixPtr++ = ray.u;
                *pixPtr++ = ray.v;
                *pixPtr++ = 0.f;
            } else {
                *pixPtr++ = 0.f;
                *pixPtr++ = 0.f;
                *pixPtr++ = 0.f;
            }
        }

    OpenImageIO::ImageOutput *out = OpenImageIO::ImageOutput::create(c_Filename);
    if(!out)
        return 1;
    OpenImageIO::ImageSpec spec(c_XRes, c_YRes, c_Channels, OpenImageIO::TypeDesc::HALF);
    out->open(c_Filename, spec);
    out->write_image(OpenImageIO::TypeDesc::FLOAT, pixels);
    out->close();
    delete out;
    rtcExit();
    return 0;
}
