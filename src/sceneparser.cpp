#include "sceneparser.h"
#include "scene.h"
#include "objloader.h"
#include "blockedrenderer.h"
#include "bsdfs/lambertian.h"
#include "shapes/sphere.h"
#include "lights/point.h"
#include "samplers/random.h"
#include "textures/constant.h"
#include "textures/bitmap.h"

#include <iostream>
#include <sstream>
#include <thread>

namespace yamcr {

std::shared_ptr<Renderer> SceneParser::Parse(const std::string &filename) {
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
    std::shared_ptr<Renderer> renderer;
    if(result) {
        renderer = ParseRenderer(doc.child("renderer"));
    } else {
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << std::endl;
        throw std::runtime_error("Parse error");
    }
    return renderer;
}

std::shared_ptr<Scene> SceneParser::ParseScene(pugi::xml_node node) {
    std::vector<std::shared_ptr<Primitive>> prims;
    std::vector<std::shared_ptr<Light>> lights;
    for(auto child: node.children()) {
        std::string name = child.name();
        if(name == "primitive") {
            prims.push_back(ParsePrimitive(child));
        } else if(name == "light") {
            lights.push_back(ParseLight(child));
        } else {
            std::cerr << "Unrecognized tag:[" << name << "]" << std::endl;
        }
    }
    std::shared_ptr<Scene> scene = 
        std::make_shared<Scene>(prims, lights);
    return scene;
}

std::shared_ptr<Primitive> SceneParser::ParsePrimitive(pugi::xml_node node) {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<BSDF> bsdf;    
    for(auto child: node.children()) {
        std::string name = child.name();
        if(name == "shape") {
            shape = ParseShape(child);
        } else if(name == "bsdf") {
            bsdf = ParseBSDF(child);
        } else {
            std::cerr << "Unrecognized tag:[" << name << "]" << std::endl;
        }
    }
    std::shared_ptr<Primitive> prim = 
        std::make_shared<Primitive>(shape, bsdf);
    return prim;
}

std::shared_ptr<Shape> SceneParser::ParseShape(pugi::xml_node node) {
    Eigen::Affine3f transform = ParseTransform(node);
    std::string type = node.attribute("type").value();
    std::shared_ptr<Shape> shape;
    if(type == "obj") {
        bool computeNormal = ParseBool(node.attribute("computenormal"), true);
        std::string filename = ParseString(node.attribute("filename"), "");
        ObjLoader loader(computeNormal);
        try {
            shape = loader.Load(transform, filename);
        } catch(std::exception &ex) {
            std::cerr << "Error from obj loader:" << ex.what() << std::endl;
            shape = std::shared_ptr<Shape>();
        }
    } else if(type == "trianglemesh") {       
        std::vector<PointA> vertices;
        ParseVectorPointA(node.attribute("vertices"), transform, vertices);
        std::vector<Normal> normals;
        ParseVectorNormal(node.attribute("normals"), transform, normals);
        std::vector<Point2> sts;
        ParseVectorPoint2(node.attribute("sts"), sts);
        std::vector<TriangleMesh::Triangle> indices;
        ParseVectorTriangle(node.attribute("indices"), indices);
        shape = std::make_shared<TriangleMesh>(vertices, normals, sts, indices); 
    } else if(type == "sphere") {
        Point center = ParsePoint(node.attribute("center"), Point(0.f, 0.f, 0.f));
        float radius = ParseFloat(node.attribute("radius"), 1.f);
        shape = std::make_shared<Sphere>(center, radius);
    } else {
        std::cerr << "Unrecognized \"type\" attribute in \"shape\" tag" << std::endl;
    }
    return shape;
}

std::shared_ptr<BSDF> SceneParser::ParseBSDF(pugi::xml_node node) {    
    std::string type = node.attribute("type").value();
    std::shared_ptr<BSDF> bsdf;
    if(type == "lambertian") {                
        std::shared_ptr<TextureSpectrum> kdTexture;
        pugi::xml_node kdNode = node.child("Kd");
        if(!kdNode.empty()) {            
            kdTexture = ParseTextureSpectrum(kdNode);
        } else {            
            kdTexture = std::make_shared<ConstantTextureSpectrum>(
                    RGBSpectrum(0.5f, 0.5f, 0.5f)); 
        }
        bsdf = std::make_shared<Lambertian>(kdTexture);
    } else {
        std::cerr << "Unrecognized \"type\" attribute in \"bsdf\" tag" << std::endl;
    }
    return bsdf;
}

std::shared_ptr<Light> SceneParser::ParseLight(pugi::xml_node node) {
    std::string type = node.attribute("type").value();
    std::shared_ptr<Light> light;
    if(type == "point") {
        Point from = ParsePoint(node.attribute("from"), Point(0.f, 0.f, 0.f));
        RGBSpectrum intensity = ParseColor(node.attribute("intensity"), RGBSpectrum(1.f, 1.f, 1.f));
        light = std::make_shared<PointLight>(from, intensity);
    } else {
        std::cerr << "Unrecognized \"type\" attribute of \"light\" tag" << std::endl;
    }
    return light;
}

std::shared_ptr<Camera> SceneParser::ParseCamera(pugi::xml_node node) {
    Point pos = ParsePoint(node.attribute("position"), Point(0.f, 0.f, 0.f));   
    Point target = ParsePoint(node.attribute("target"), Point(0.f, 0.f, -1.f));
    Vector up = ParseVector(node.attribute("up"), Vector(0.f, 1.f, 0.f)).normalized();
    Vector dir = (target - pos).normalized();
    float fov = ParseFloat(node.attribute("fov"), 45.f);
    std::shared_ptr<Film> film = ParseFilm(node.child("film"));
    std::shared_ptr<Camera> camera = 
        std::make_shared<Camera>(pos, dir, up, fov, film);
    return camera;
}

std::shared_ptr<Film> SceneParser::ParseFilm(pugi::xml_node node) {
    int width = ParseInteger(node.attribute("width"),  512);
    int height = ParseInteger(node.attribute("height"), 512);
    std::string filename = ParseString(node.attribute("filename"), "foo.exr");
    std::shared_ptr<Film> film = 
        std::make_shared<Film>(width, height, filename);
    return film;
}

std::shared_ptr<Sampler> SceneParser::ParseSampler(pugi::xml_node node) {
    std::string type = ParseString(node.attribute("type"), "random");
    std::shared_ptr<Sampler> sampler;
    if(type == "random") {
        sampler = std::make_shared<RandomSampler>();
    }
    return sampler;
}

int GetNumThreads() {
    int numThreads = std::thread::hardware_concurrency();
    if(numThreads > 0) {
        return numThreads;
    } 
    std::cout << "Warning: unable to detect number of cores, using 16 threads" << std::endl;
    return 16;
}

std::shared_ptr<Renderer> SceneParser::ParseRenderer(pugi::xml_node node) {
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Sampler> sampler;

    pugi::xml_node sceneNode = node.child("scene");
    if(!sceneNode.empty()) {
        scene = ParseScene(sceneNode);
    } else {
        // print something here
    }
    pugi::xml_node cameraNode = node.child("camera");
    if(!cameraNode.empty()) {
        camera = ParseCamera(cameraNode);
    } else {
        // print something here
    }
    pugi::xml_node samplerNode = node.child("sampler");
    if(!samplerNode.empty()) {
        sampler = ParseSampler(samplerNode);
    } else {
        // print something here
    }
    
    std::shared_ptr<Renderer> renderer;
    std::string type = ParseString(node.attribute("type"), "blocked");
    if(type == "blocked") {
        int blocksize = ParseInteger(node.attribute("blocksize"), 32);
        int spp = ParseInteger(node.attribute("spp"), 16);
        int numThreads = ParseInteger(node.attribute("numthreads"), 0);
        if(numThreads == 0)
            numThreads = GetNumThreads();
    
        renderer = std::make_shared<BlockedRenderer>(scene, camera, sampler,
            blocksize, spp, numThreads);
    } else {
        // print something here
    }
    return renderer;
}

std::shared_ptr<TextureSpectrum> SceneParser::ParseTextureSpectrum(pugi::xml_node node) {
    std::string type = node.attribute("type").value();
    std::shared_ptr<TextureSpectrum> texture;
    if(type == "constant") {
        RGBSpectrum color(0.f);
        pugi::xml_attribute colorAttr = node.attribute("color");
        if(!colorAttr.empty()) {
            std::stringstream ss(colorAttr.value());
            float r, g, b;
            if(ss >> r >> g >> b)
                color = RGBSpectrum(r, g, b);
        }
        texture = std::make_shared<ConstantTextureSpectrum>(color);
    } else if(type == "bitmap") {
        std::string filename;
        pugi::xml_attribute filenameAttr = node.attribute("filename");
        if(!filenameAttr.empty())
            filename = filenameAttr.value();
        float sScale = 1.f, tScale = 1.f;
        pugi::xml_attribute ssAttr = node.attribute("sscale");
        if(!ssAttr.empty()) 
            sScale = std::stof(ssAttr.value());
        pugi::xml_attribute tsAttr = node.attribute("tscale");
        if(!tsAttr.empty())
            tScale = std::stof(tsAttr.value());
        texture =
            std::make_shared<BitmapTextureSpectrum>(filename, Vector2(sScale, tScale));
    } else {
        std::cerr << "Unrecognized \"type\" attribute in \"texture\" tag" << std::endl;
    }
    return texture;
}

Eigen::Affine3f SceneParser::ParseTransform(pugi::xml_node node) {
    Eigen::Affine3f transform = Eigen::Affine3f::Identity();
    for(auto child: node.children()) {
        std::string name = child.name();
        if(name == "translate") {
            float x = std::stof(child.attribute("x").value());
            float y = std::stof(child.attribute("y").value());
            float z = std::stof(child.attribute("z").value());
            transform *= Eigen::Translation3f(x, y, z);
        } else if(name == "scale") {
            float x = std::stof(child.attribute("x").value());
            float y = std::stof(child.attribute("y").value());
            float z = std::stof(child.attribute("z").value());
            transform *= Eigen::Scaling(x, y, z);
        }
    }
    return transform;
}

RGBSpectrum SceneParser::ParseColor(pugi::xml_attribute attr, const RGBSpectrum &defau) {
    if(attr.empty()) 
        return defau;
    
    RGBSpectrum result = defau;
    std::stringstream ss(attr.value());
    float r, g, b;
    if(ss >> r >> g >> b)
        result = RGBSpectrum(r, g, b);
    return result;
}

Point SceneParser::ParsePoint(pugi::xml_attribute attr, const Point &defau) {
    if(attr.empty())
        return defau;

    Point result = defau;
    std::stringstream ss(attr.value());
    float x, y, z;
    if(ss >> x >> y >> z) 
        result = Point(x, y, z);            
    return result;
}

Vector SceneParser::ParseVector(pugi::xml_attribute attr, const Vector &defau) {
    if(attr.empty())
        return defau;

    Vector result = defau;
    std::stringstream ss(attr.value());
    float x, y, z;
    if(ss >> x >> y >> z) 
        result = Vector(x, y, z);            
    return result;
}


int SceneParser::ParseInteger(pugi::xml_attribute attr, int defau) {
    if(attr.empty()) 
        return defau;

    int result = defau;
    std::stringstream ss(attr.value());
    if(!(ss >> result)) {
        //print something here
    }
    return result;
}

float SceneParser::ParseFloat(pugi::xml_attribute attr, float defau) {
    if(attr.empty()) 
        return defau;

    float result = defau;
    std::stringstream ss(attr.value());
    if(!(ss >> result)) {
        //print something here
    }
    return result;
}

bool SceneParser::ParseBool(pugi::xml_attribute attr, bool defau) {
    if(attr.empty()) 
        return defau;
    
    bool result = defau;
    if(std::string(attr.value()) == "true") {
        result = true;
    } else if(std::string(attr.value()) == "false") {
        result = false;
    }
    return result;    
}

std::string SceneParser::ParseString(pugi::xml_attribute attr, const std::string &defau) {
    if(attr.empty())
        return defau;

    std::string result = attr.value();
    return result;
}

void SceneParser::ParseVectorPointA(pugi::xml_attribute attr, const Eigen::Affine3f &transform, std::vector<PointA> &result) {
    if(attr.empty()) 
        return;

    std::stringstream ss(attr.value());
    float x, y, z;
    while(ss >> x >> y >> z) 
        result.push_back(transform*PointA(x, y, z));           
}

void SceneParser::ParseVectorNormal(pugi::xml_attribute attr, const Eigen::Affine3f &transform, std::vector<Normal> &result) {
    if(attr.empty())
        return;

    std::stringstream ss(attr.value());
    float x, y, z;
    while(ss >> x >> y >> z)
        result.push_back((transform*Normal(x, y, z)).normalized());
}

void SceneParser::ParseVectorPoint2(pugi::xml_attribute attr, std::vector<Point2> &result) {
    if(attr.empty())
        return;

    std::stringstream ss(attr.value());
    float x, y;
    while(ss >> x >> y)
        result.push_back(Point2(x, y));
}

void SceneParser::ParseVectorTriangle(pugi::xml_attribute attr, std::vector<TriangleMesh::Triangle> &result) {
    if(attr.empty())
        return;

    std::stringstream ss(attr.value());
    int a, b, c;
    while(ss >> a >> b >> c)
        result.push_back(TriangleMesh::Triangle(a, b, c));
}

}
