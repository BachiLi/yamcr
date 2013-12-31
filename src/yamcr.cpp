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
#include "film.h"
#include "pointlight.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512;
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

void CreateLights(std::vector<std::shared_ptr<PointLight>> &lights) {
    lights.push_back(std::make_shared<PointLight>(
                Point(0.f, 0.f, 0.f),
                RGBSpectrum(100.f, 100.f, 100.f)));
}

int main(int argc, char *argv[]) {
    rtcInit(NULL);
    std::vector<std::shared_ptr<TriangleMesh>> shapes;
    CreateShapes(shapes);
    Scene scene(shapes);
    Camera camera(c_XRes, c_YRes);
    Film film(c_XRes, c_YRes);

    for(int y = 0; y < c_YRes; y++)
        for(int x = 0; x < c_XRes; x++) {
            Ray ray = camera.GenerateRay(x, y);
            if(scene.Intersect(ray)) {
                film.AddSample(x, y, RGBSpectrum(ray.u, ray.v, 0.f));
            } 
        }

    film.Write(std::string(c_Filename));
    rtcExit();
    return 0;
}
