#include "commondefs.h"
#include <OpenImageIO/imageio.h>
#include <stdint.h>
#include <embree2/rtcore.h>
#include <limits>

#include "ray.h"
#include "scene.h"
#include "camera.h"
#include "trianglemesh.h"
#include "spectrum.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512;
const int c_Channels = 3;
const char *c_Filename = "foo.exr";

void CreateShapes(std::vector<std::shared_ptr<TriangleMesh>> &shapes) {
    std::vector<PointA> vertices;
    vertices.push_back(PointA( 0.f,  1.f,  1.f));
    vertices.push_back(PointA(-1.f, -1.f,  1.f));
    vertices.push_back(PointA( 1.f, -1.f,  1.f));
    std::vector<Triangle> triangles;
    triangles.push_back(Triangle(0, 1, 2));
    std::shared_ptr<TriangleMesh> mesh = 
        std::make_shared<TriangleMesh>(vertices, triangles);
    shapes.push_back(mesh);
}

int main(int argc, char *argv[]) {
    rtcInit(NULL);
    std::vector<std::shared_ptr<TriangleMesh>> shapes;
    CreateShapes(shapes);
    Scene scene(shapes);
    Camera camera(c_XRes, c_YRes);
    RGBSpectrum pixels[c_XRes*c_YRes];
    RGBSpectrum *pixPtr = pixels;
    for(int y = 0; y < c_YRes; y++)
        for(int x = 0; x < c_XRes; x++) {
            Ray ray = camera.GenerateRay(x, y);
            if(scene.Intersect(ray)) {
                *pixPtr++ = RGBSpectrum(std::array<float, 3>{{ray.u, ray.v, 0.f}});
            } else {
                *pixPtr++ = RGBSpectrum(0.f);
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
