#ifndef YAMCR_CAMERA_H__
#define YAMCR_CAMERA_H__

#include "ray.h"
#include "point.h"
#include "vector.h"

namespace yamcr {

class Camera {
public:
    Camera(const Point &pos, const Vector &up, const Vector &dir, 
           int xres, int yres);
    Ray GenerateRay(const Point2 &screenPos) const;
private:
    const Point m_Pos;
    const Vector m_Up, m_Dir, m_Right;
    const int m_XRes, m_YRes;
};

}

#endif //YAMCR_CAMERA_H__
