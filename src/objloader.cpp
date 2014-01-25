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

#include "objloader.h"

#include <fstream>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <map>

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

static std::vector<int> splitFaceStr(const std::string &s) {
    std::vector<int> result;
    size_t prev = 0;
    size_t found = s.find_first_of("/");
    while(found != std::string::npos) {
        if(found - prev > 0)
            result.push_back(std::stoi(s.substr(prev, found - prev)));
        prev = found + 1;
        found = s.find_first_of("/", found + 1);
    }
    result.push_back(std::stoi(s.substr(prev)));

    while(result.size() < 3)
        result.push_back(0);

    return result;
}

namespace yamcr {

struct ObjVertex {
    ObjVertex(const std::vector<int> &id)
        : v(id[0]-1), vt(id[1]-1), vn(id[2]-1) {}

    bool operator<(const ObjVertex &vertex) const {
        if(v != vertex.v) return v < vertex.v;
        if(vt != vertex.vt) return vt < vertex.vt;
        if(vn != vertex.vn) return vn < vertex.vn;
        return false;
    }

    int v, vt, vn;
};

size_t GetVertexId(const ObjVertex &vertex, 
        const std::vector<Point> &posPool, const std::vector<Point2> &stPool,
        std::vector<PointA> &pos, std::vector<Point2> &st, 
        std::map<ObjVertex, size_t> &vertexMap) {
    auto it = vertexMap.find(vertex);
    if(it != vertexMap.end()) {
        return it->second;
    }
    size_t id = pos.size();
    pos.push_back(PointA(posPool[vertex.v]));
    if(vertex.vt != -1)
        st.push_back(stPool[vertex.vt]);
    vertexMap[vertex] = id;
    return id;
}

std::shared_ptr<TriangleMesh> ObjLoader::Load(
        const Transform &obj2world, const std::string &filename) {
    std::vector<Point> posPool;
    std::vector<Triangle> triangles;
    std::vector<Point2> stPool;
    std::map<ObjVertex, size_t> vertexMap;

    std::vector<PointA> positions;
    std::vector<Point2> sts;

    std::ifstream ifs(filename.c_str(), std::ifstream::in);
    if(!ifs.is_open())
        throw std::runtime_error("Unable to open the obj file");
    while(ifs.good()) {
        std::string line;
        std::getline(ifs, line);
        line = trim(line);
        if(line.size() == 0 || line[0] == '#') // comment
            continue;

        std::stringstream ss(line);
        std::string token;
        ss >> token;
        if(token == "v") { //vertices
            float x, y, z, w = 1.f; 
            ss >> x >> y >> z >> w; 
            float invW = 1.f/w;
            posPool.push_back(obj2world*Point(x*invW, y*invW, z*invW));            
        } else if(token == "vt") {
            float s, t, w;
            ss >> s >> t >> w;
            stPool.push_back(Point2(s, t));
        } else if(token == "f") {
            std::string i0, i1, i2;
            ss >> i0 >> i1 >> i2;
            std::vector<int> i0f = splitFaceStr(i0);
            std::vector<int> i1f = splitFaceStr(i1);
            std::vector<int> i2f = splitFaceStr(i2);

            ObjVertex v0(i0f), v1(i1f), v2(i2f);
            size_t v0id = GetVertexId(v0, posPool, stPool, positions, sts, vertexMap);
            size_t v1id = GetVertexId(v1, posPool, stPool, positions, sts, vertexMap);
            size_t v2id = GetVertexId(v2, posPool, stPool, positions, sts, vertexMap);            
            triangles.push_back(Triangle(v0id, v1id, v2id));

            std::string i3;
            if(ss >> i3) {
                std::vector<int> i3f = splitFaceStr(i3);
                ObjVertex v3(i3f);               
                size_t v3id = GetVertexId(v3, posPool, stPool, positions, sts, vertexMap);
                triangles.push_back(Triangle(v3id, v0id, v2id));
            }
            std::string i4;
            if(ss >> i4) 
                throw std::runtime_error("The object file contains n-gon (n>4) that we do not support.");
        } // Currently ignore other tokens
    }

    std::vector<Normal> normals;
    if(m_ComputeNormals)
        ComputeNormals(positions, triangles, normals);
    return std::make_shared<TriangleMesh>(positions, normals, sts, triangles);
}

// Numerical robust computation of angle between unit vectors
template <typename VectorType> inline float UnitAngle(const VectorType &u, const VectorType &v) {
    if(u.dot(v) < 0)
        return M_PI - 2 * std::asin(0.5f * (v+u).norm());
    else
        return 2 * std::asin(0.5f * (v-u).norm());
}

void ObjLoader::ComputeNormals(const std::vector<PointA> &vertices, const std::vector<Triangle> &triangles, 
        std::vector<Normal> &normals) {
    normals.resize(vertices.size(), Normal(0.f));

    //Nelson Max, "Computing Vertex Normals from Facet Normals", 1999
    for(auto &tri : triangles) {
        Normal n(0.f);
        for (int i=0; i<3; ++i) {
            const PointA &v0 = vertices[tri.idx[i]];
            const PointA &v1 = vertices[tri.idx[(i+1)%3]];
            const PointA &v2 = vertices[tri.idx[(i+2)%3]];
            Vector sideA(v1-v0), sideB(v2-v0);
            if (i==0) {
                n = sideA.cross(sideB);
                float length = n.norm();
                if (length == 0)
                    break;
                n /= length;
            }
            float angle = UnitAngle(sideA.normalized(), sideB.normalized());
            normals[tri.idx[i]] += n * angle;            
        }
    }
    
    for(auto &n : normals) {
        float length = n.norm();
        if (length != 0) {
            n /= length;
        } else {
            /* Choose some bogus value */
            n = Normal(0, 0, 0);
        }
    }
}

}
