#include "commondefs.h"
#include <OpenImageIO/imageio.h>
#include <stdint.h>
#include <embree2/rtcore.h>
#include <limits>

#include "ray.h"
#include "scene.h"
#include "camera.h"
#include "shapes/trianglemesh.h"
#include "lights/point.h"
#include "samplers/random.h"
#include "spectrum.h"
#include "film.h"
#include "intersection.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512, c_Spp = 16;
const char *c_Filename = "foo.exr";
const Point c_CameraPos = Point(0.f, 0.f, -5.f);
const Vector c_CameraDir = Vector(0.f, 0.f, 1.f);
const Vector c_CameraUp = Vector(0.f, 1.f, 0.f);

void CreateShapes(std::vector<std::shared_ptr<Shape>> &shapes) {
    {
        std::vector<PointA> vertices;
        vertices.push_back(PointA( 0.0f,  0.5f,  1.0f));
        vertices.push_back(PointA(-0.5f, -0.25f,  0.2f));
        vertices.push_back(PointA( 0.5f,  0.0f,  0.6f));
        std::vector<Triangle> triangles;
        triangles.push_back(Triangle(0, 1, 2));
        std::shared_ptr<TriangleMesh> mesh = 
            std::make_shared<TriangleMesh>(vertices, triangles);
        shapes.push_back(mesh);
    }

    {
        std::vector<PointA> vertices;
        vertices.push_back(PointA(-4.f, -1.5f,  10.f));
        vertices.push_back(PointA(-4.f, -1.5f,  0.f));
        vertices.push_back(PointA( 4.f, -1.5f,  0.f));
        vertices.push_back(PointA( 4.f, -1.5f,  10.f));
        std::vector<Triangle> triangles;
        triangles.push_back(Triangle(0, 1, 2));
        triangles.push_back(Triangle(0, 2, 3));
        std::shared_ptr<TriangleMesh> mesh = 
            std::make_shared<TriangleMesh>(vertices, triangles);
        shapes.push_back(mesh);
    }
}

void CreateLights(std::vector<std::shared_ptr<Light>> &lights) {
    lights.push_back(std::make_shared<PointLight>(
                Point(4.f, 4.f, -1.0f),
                RGBSpectrum(20.f, 20.f, 20.f)));
}

int main(int argc, char *argv[]) {
    rtcInit(NULL);
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Light>> lights;
    CreateShapes(shapes);
    CreateLights(lights);
    Scene scene(shapes);
    Camera camera(c_CameraPos, c_CameraDir, c_CameraUp, c_XRes, c_YRes);
    Film film(c_XRes, c_YRes);
    RandomSampler random;

    for(int y = 0; y < c_YRes; y++) {
        for(int x = 0; x < c_XRes; x++) {            
            random.NewSequence();
            for(int s = 0; s < c_Spp; s++) {
                Ray ray = camera.GenerateRay(Point2(x, y) + random.Next2D());
                Intersection isect;
                RGBSpectrum L(0.f);
                if(scene.Intersect(ray, &isect)) {
                    for(auto it = lights.begin(); it != lights.end(); it++) {
                        Ray shadowRay;
                        RGBSpectrum Le = (*it)->SampleDirect(isect, shadowRay);
                        if(!scene.Occluded(shadowRay)) {
                            L = Le*AbsDot(isect.n, shadowRay.dir);
                        }                        
                    }                
                } 
                film.AddSample(x, y, L);
            }
        }
    }

    film.Write(std::string(c_Filename));
    rtcExit();
    return 0;
}
