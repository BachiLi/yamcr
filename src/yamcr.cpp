#include "commondefs.h"
#include <OpenImageIO/imageio.h>
#include <stdint.h>
#include <embree2/rtcore.h>
#include <limits>

#include "ray.h"

using namespace yamcr;

const int c_XRes = 512, c_YRes = 512;
const int c_Channels = 3;
const char *c_Filename = "foo.exr";

struct Vertex {float x, y, z, a;};
struct Triangle {int v0, v1, v2;};

RTCScene CreateScene();
RTCRay GenerateRay(int x, int y);
bool Intersect(const RTCScene scene, RTCRay &ray);

RTCScene CreateScene() {
    RTCScene scene = rtcNewScene(RTC_SCENE_STATIC, 
            RTC_INTERSECT1);
    
    unsigned geomID = rtcNewTriangleMesh(scene, RTC_GEOMETRY_STATIC, 1, 3, 1);
    Vertex* vertices = (Vertex*) rtcMapBuffer(scene, geomID, RTC_VERTEX_BUFFER);
    vertices[0] = {.x =  0.f, .y =  1.f, .z = 1.f};
    vertices[1] = {.x = -1.f, .y = -1.f, .z = 1.f};
    vertices[2] = {.x =  1.f, .y = -1.f, .z = 1.f};
    rtcUnmapBuffer(scene, geomID, RTC_VERTEX_BUFFER);

    Triangle* triangles = (Triangle*) rtcMapBuffer(scene, geomID, RTC_INDEX_BUFFER);
    triangles[0] = {.v0 = 0, .v1 = 1, .v2 = 2};
    rtcUnmapBuffer(scene, geomID, RTC_INDEX_BUFFER);

    rtcCommit(scene);
    
    return scene;
}   

RTCRay GenerateRay(int x, int y) {
    float nx = (((float)x+0.5f)/(float)c_XRes) - 0.5f;
    float ny = - ((((float)y+0.5f)/(float)c_YRes) - 0.5f);
    Ray ray(Point(0.f, 0.f, -5.f),
            Vector(nx, ny, 1.f));
    return ray.ToRTCRay();
}

bool Intersect(RTCScene scene, RTCRay &ray) {
    rtcIntersect(scene, ray);
    return ray.geomID != -1;
}

int main(int argc, char *argv[]) {
    rtcInit(NULL);
    RTCScene scene = CreateScene();
    float pixels[c_XRes*c_YRes*c_Channels] = {0.f};
    float *pixPtr = pixels;
    for(int y = 0; y < c_YRes; y++)
        for(int x = 0; x < c_XRes; x++) {
            RTCRay ray = GenerateRay(x, y);
            if(Intersect(scene, ray)) {
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
    rtcDeleteScene(scene);
    rtcExit();
    return 0;
}
