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

#include "commondefs.h"
#include <OpenImageIO/imageio.h>
#include <OpenImageIO/imagebufalgo.h>
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
#include "textures/constant.h"
#include "textures/bitmap.h"
#include "spectrum.h"
#include "film.h"
#include "intersection.h"
#include "blockedrenderer.h"
#include "objloader.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512, c_BlockSize = 32, c_Spp = 16;
const char *c_Filename = "foo.exr";
const Point c_CameraPos = Point(0.f, 1.f, -5.f);
const Vector c_CameraDir = (Point(0.f, 0.f, -1.f) - c_CameraPos).normalized();
const Vector c_CameraUp = Vector(0.f, 1.f, 0.f);

void CreatePrimitives(std::vector<std::shared_ptr<Primitive>> &prims) {
    {
        ObjLoader loader(true);
        std::shared_ptr<TriangleMesh> shape =
            loader.Load(Eigen::Affine3f(Eigen::Translation3f(0.f, -1.5f, 0.f)*Eigen::Scaling(1.f/50.f)), "models/teapot.obj");
        float dark[] = {0.1f, 0.1f, 0.1f};
        float light[] = {0.75f, 0.25f, 0.25f};
        OpenImageIO::ImageBuf cb(OpenImageIO::ImageSpec(512, 512, 3, OpenImageIO::TypeDesc::FLOAT));
        OpenImageIO::ImageBufAlgo::checker(cb, 32, 32, 1, dark, light);
        const std::string cbName = std::string("checker.exr");
        cb.write(cbName);
        std::shared_ptr<TextureSpectrum> texture = 
            std::make_shared<BitmapTextureSpectrum>(cbName, Vector2(2.f, 2.f));

        std::shared_ptr<BSDF> bsdf =
            std::make_shared<Lambertian>(texture);
        std::shared_ptr<Primitive> prim =
            std::make_shared<Primitive>(shape, bsdf);
        prims.push_back(prim);       
    }

    {
        std::vector<PointA> vertices;
        vertices.push_back(PointA(-20.f, -1.5f, 15.f));
        vertices.push_back(PointA(-20.f, -1.5f, -5.f));
        vertices.push_back(PointA( 20.f, -1.5f, -5.f));
        vertices.push_back(PointA( 20.f, -1.5f, 15.f));
        std::vector<Triangle> triangles;
        triangles.push_back(Triangle(0, 1, 2));
        triangles.push_back(Triangle(0, 2, 3));
        std::shared_ptr<TriangleMesh> shape = 
            std::make_shared<TriangleMesh>(vertices, std::vector<Normal>(), std::vector<Point2>(), triangles);
        std::shared_ptr<TextureSpectrum> texture = 
            std::make_shared<ConstantTextureSpectrum>(RGBSpectrum(0.25f, 0.75f, 0.25f)); 
        std::shared_ptr<BSDF> bsdf = 
            std::make_shared<Lambertian>(texture);
        std::shared_ptr<Primitive> prim =
            std::make_shared<Primitive>(shape, bsdf);
        prims.push_back(prim);
    }
}

void CreateLights(std::vector<std::shared_ptr<Light>> &lights) {
    lights.push_back(std::make_shared<PointLight>(
                Point(10.f, 10.f, -1.0f),
                RGBSpectrum(1000.f, 1000.f, 1000.f)));
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
