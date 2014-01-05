#ifndef YAMCR_INTERSECTION_H__
#define YAMCR_INTERSECTION_H__

#include "point.h"
#include "normal.h"

namespace yamcr {

struct Intersection {    
    Point p;
    Normal n;
    float rayEpsilon;
};

}

#endif //#ifndef YAMCR_INTERSECTION_H__
