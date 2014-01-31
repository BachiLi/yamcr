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
    
    Normal &Ng = isect.Ng;    
    float u = isect.uv[0], v = isect.uv[1], w = 1.0f-u-v;
    Point2 st0, st1, st2;
    if(m_STs.size() == 0) {
        st0 = Point2(0.f, 0.f);
        st1 = Point2(1.f, 0.f);
        st2 = Point2(0.f, 1.f);
        isect.st = isect.uv;
    } else {        
        st0 = m_STs[tri.idx[0]];
        st1 = m_STs[tri.idx[1]];
        st2 = m_STs[tri.idx[2]];
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

    if(Ng.squaredNorm() <= 0.f) { 
        // Degenerated triangle
        isect.dPds = Vector(0.f);
        isect.dPdt = Vector(0.f);
    } else {
        const PointA &p0 = m_Vertices[tri.idx[0]];
        const PointA &p1 = m_Vertices[tri.idx[1]];
        const PointA &p2 = m_Vertices[tri.idx[2]];

        Vector dP1 = p1 - p0, dP2 = p2 - p0;
        Vector2 dST1 = st1 - st0, dST2 = st2 - st0;

		float determinant = dST1[0] * dST2[1] - dST1[1] * dST2[0];
        if(determinant == 0.f) {
            // Degenerated st
            CoordinateSystem(Ng, isect.dPds, isect.dPdt);
        } else {
            float invDet = Rcp(determinant);
            isect.dPds = ( dST2[1] * dP1 - dST1[1] * dP2)*invDet;
            isect.dPdt = (-dST2[0] * dP1 + dST1[0] * dP2)*invDet;
        }       
    }
}

}
