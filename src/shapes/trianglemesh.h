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

#ifndef YAMCR_SHAPES_TRIANGLEMESH_H__
#define YAMCR_SHAPES_TRIANGLEMESH_H__

#include "shape.h"
#include "vector.h"
#include <vector>

namespace yamcr {
class TriangleMesh : public Shape {
public:

    struct Triangle {
        Triangle(uint32_t id0, uint32_t id1, uint32_t id2) {
            idx[0] = id0;
            idx[1] = id1;
            idx[2] = id2;
        }
        // Indices to vertex buffer    
        uint32_t idx[3];    
    };


    TriangleMesh(const std::vector<PointA> &vertices,
                 const std::vector<Normal> &normals,
                 const std::vector<Point2> &sts,
                 const std::vector<Triangle> &triangles) :
        m_Vertices(vertices), m_Normals(normals), m_STs(sts), m_Triangles(triangles) {}

    void Register(RTCScene rtcScene, unsigned int geomID);
    void PostIntersect(Intersection &isect) const;
private:
    std::vector<PointA> m_Vertices;
    std::vector<Normal> m_Normals;
    std::vector<Point2> m_STs;
    std::vector<Triangle> m_Triangles;
};

}

#endif //YAMCR_SHAPES_TRIANGLEMESH_H__
