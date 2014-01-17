#include "commondefs.h"
#include <OpenImageIO/imageio.h>
#include <stdint.h>
#include <embree2/rtcore.h>
#include <limits>
#include <exception>
#include <thread>

#include "ray.h"
#include "scene.h"
#include "camera.h"
#include "shapes/sphere.h"
#include "shapes/trianglemesh.h"
#include "lights/point.h"
#include "samplers/random.h"
#include "bsdfs/lambertian.h"
#include "spectrum.h"
#include "film.h"
#include "intersection.h"
#include "blockedrenderer.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512, c_BlockSize = 32, c_Spp = 512;
const char *c_Filename = "foo.exr";
const Point c_CameraPos = Point(0.f, 0.f, -5.f);
const Vector c_CameraDir = Vector(0.f, 0.f, 1.f);
const Vector c_CameraUp = Vector(0.f, 1.f, 0.f);

void CreatePrimitives(std::vector<std::shared_ptr<Primitive>> &prims) {
    {
        std::shared_ptr<Sphere> shape =
            std::make_shared<Sphere>(Point(0.f, 0.f, 0.5f), 0.5f);
        std::shared_ptr<BSDF> bsdf =
            std::make_shared<Lambertian>(RGBSpectrum(0.75f, 0.25f, 0.25f));
        std::shared_ptr<Primitive> prim =
            std::make_shared<Primitive>(shape, bsdf);
        prims.push_back(prim);
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
        std::shared_ptr<TriangleMesh> shape = 
            std::make_shared<TriangleMesh>(vertices, triangles);
        std::shared_ptr<BSDF> bsdf = 
            std::make_shared<Lambertian>(RGBSpectrum(0.25f, 0.75f, 0.25f));
        std::shared_ptr<Primitive> prim =
            std::make_shared<Primitive>(shape, bsdf);
        prims.push_back(prim);
    }
}

void CreateLights(std::vector<std::shared_ptr<Light>> &lights) {
    lights.push_back(std::make_shared<PointLight>(
                Point(4.f, 4.f, -1.0f),
                RGBSpectrum(70.f, 70.f, 70.f)));
    lights.push_back(std::make_shared<PointLight>(
                Point(0.f, 0.f, -5.f),
                RGBSpectrum(5.f, 5.f, 5.f)));
}

int GetNumThreads() {
    int numThreads = std::thread::hardware_concurrency();
    if(numThreads > 0) {
        return numThreads;
    } 
    std::cout << "Warning: unable to detect number of cores, using 16 threads" << std::endl;
    return 16;
}

int main(int argc, char *argv[]) {
    try {
        rtcInit(NULL);
        std::vector<std::shared_ptr<Primitive>> prims;
        std::vector<std::shared_ptr<Light>> lights;
        CreatePrimitives(prims);
        CreateLights(lights);
        std::shared_ptr<Scene> scene = 
            std::make_shared<Scene>(prims, lights);
        std::shared_ptr<Camera> camera = 
            std::make_shared<Camera>(c_CameraPos, c_CameraDir, c_CameraUp, c_XRes, c_YRes);
        std::shared_ptr<Film> film = 
            std::make_shared<Film>(c_XRes, c_YRes, c_Filename);
        std::shared_ptr<Sampler> sampler =
            std::make_shared<RandomSampler>();
        const int numThreads = GetNumThreads();
        std::shared_ptr<BlockedRenderer> renderer = 
            std::make_shared<BlockedRenderer>(scene, camera, film, sampler, 
                    c_BlockSize, c_Spp, numThreads);
        renderer->Render();
        rtcExit();
    } catch(std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
