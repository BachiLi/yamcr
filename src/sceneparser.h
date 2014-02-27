#ifndef YAMCR_SCENE_PARSER_H__
#define YAMCR_SCENE_PARSER_H__

#include <memory>
#include <pugixml.hpp>
#include <Eigen/Dense>

#include "vector.h"
#include "shapes/trianglemesh.h"
#include "textures/texture.h"

namespace yamcr {

class Scene;
class Primitive;
class Shape;
class BSDF;
class Light;
class Camera;
class Sampler;
class Film;
class Renderer;
class RGBSpectrum;

class SceneParser {
public:
    std::shared_ptr<Renderer> Parse(const std::string &filename);
private:
    std::shared_ptr<Scene> ParseScene(pugi::xml_node node);
    std::shared_ptr<Primitive> ParsePrimitive(pugi::xml_node node);
    std::shared_ptr<Shape> ParseShape(pugi::xml_node node);
    std::shared_ptr<BSDF> ParseBSDF(pugi::xml_node node);
    std::shared_ptr<Light> ParseLight(pugi::xml_node node);
    std::shared_ptr<Camera> ParseCamera(pugi::xml_node node);
    std::shared_ptr<Film> ParseFilm(pugi::xml_node node);
    std::shared_ptr<Sampler> ParseSampler(pugi::xml_node node);
    std::shared_ptr<Renderer> ParseRenderer(pugi::xml_node node);
    std::shared_ptr<TextureSpectrum> ParseTextureSpectrum(pugi::xml_node node);

    Eigen::Affine3f ParseTransform(pugi::xml_node node);
    RGBSpectrum ParseColor(pugi::xml_attribute attr, const RGBSpectrum &defau);
    Point ParsePoint(pugi::xml_attribute attr, const Point &defau);
    Vector ParseVector(pugi::xml_attribute attr, const Vector &defau);
    int ParseInteger(pugi::xml_attribute attr, int defau);
    float ParseFloat(pugi::xml_attribute attr, float defau);
    bool ParseBool(pugi::xml_attribute attr, bool defau);    
    std::string ParseString(pugi::xml_attribute attr, const std::string &defau);
    void ParseVectorPointA(pugi::xml_attribute attr, const Eigen::Affine3f &transform, std::vector<PointA> &result);
    void ParseVectorNormal(pugi::xml_attribute attr, const Eigen::Affine3f &transform, std::vector<Normal> &result);
    void ParseVectorPoint2(pugi::xml_attribute attr, std::vector<Point2> &result);
    void ParseVectorTriangle(pugi::xml_attribute attr, std::vector<TriangleMesh::Triangle> &result);

    pugi::xml_document doc;
};

}

#endif //YAMCR_SCENE_PARSER_H__
