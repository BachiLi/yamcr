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

#include "trianglemesh.h"

#include "fastmath.h"

#include <embree2/rtcore.h>
#include <algorithm>
#include <stdexcept>

namespace yamcr {

void TriangleMesh::Register(RTCScene rtcScene, unsigned int geomID) {
    unsigned int obtainedGeomID = rtcNewTriangleMesh(rtcScene, RTC_GEOMETRY_STATIC, 
            m_Triangles.size(), m_Vertices.size(), 1);
    if(obtainedGeomID != geomID)
        throw std::runtime_error("Wrong geomID provided when registering a TriangleMesh");
    PointA* rtcVertices = (PointA*) rtcMapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);
    std::copy(m_Vertices.begin(), m_Vertices.end(), rtcVertices);
    rtcUnmapBuffer(rtcScene, geomID, RTC_VERTEX_BUFFER);

    Triangle* rtcTriangles = (Triangle*) rtcMapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
    std::copy(m_Triangles.begin(), m_Triangles.end(), rtcTriangles);
    rtcUnmapBuffer(rtcScene, geomID, RTC_INDEX_BUFFER);
}

void TriangleMesh::PostIntersect(Intersection &isect) const {
    const Triangle &tri = m_Triangles[isect.primID];
    //const PointA &p0 = m_Vertices[tri.idx[0]];
    //const PointA &p1 = m_Vertices[tri.idx[1]];
    //const PointA &p2 = m_Vertices[tri.idx[2]];
    const Normal &Ng = isect.Ng;    
    float u = isect.uv[0], v = isect.uv[1], w = 1.0f-u-v;
    if(m_STs.size() == 0) {
        isect.st = isect.uv;
    } else {        
        const Point2 &st0 = m_STs[tri.idx[0]];
        const Point2 &st1 = m_STs[tri.idx[1]];
        const Point2 &st2 = m_STs[tri.idx[2]];  
        isect.st = w*st0 + u*st1 + v*st2;
    }
    
    if(m_Normals.size() == 0) {
        isect.Ns = Ng; 
    } else {
        const Normal &n0 = m_Normals[tri.idx[0]];
        const Normal &n1 = m_Normals[tri.idx[1]];
        const Normal &n2 = m_Normals[tri.idx[2]];
        //Vector dPdu = p1 - p0, dPdv = p2 - p2;
        Normal Ns = w*n0 + u*n1 + v*n2;
        float sqLen = Ns.squaredNorm();
        //Ns = sqLen > 0.f ? Ns*Rsqrt(sqLen) : isect.Ng;
        if(sqLen > 0.f)
            Ns *= Rsqrt(sqLen);
        else
            Ns = isect.Ng;
        if (Ns.dot(Ng) < 0.f) 
            Ns = -Ns;
        isect.Ns = Ns;
    }

}

}
