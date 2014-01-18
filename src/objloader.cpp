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
    if(found == std::string::npos) {
        result.push_back(std::stoi(s));
        return result;
    }
    while(found != std::string::npos) {
        if(found - prev > 0)
            result.push_back(std::stoi(s.substr(prev, found - prev)));
        prev = found + 1;
        found = s.find_first_of("/", found + 1);
    }
    return result;
}

namespace yamcr {

std::shared_ptr<TriangleMesh> ObjLoader::Load(
        const Transform &obj2world, const std::string &filename) {
    std::vector<PointA> vertices;
    std::vector<Triangle> triangles;

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
            vertices.push_back(obj2world*PointA(x*invW, y*invW, z*invW));            
        } else if(token == "f") {
            std::string i1, i2, i3;
            ss >> i1 >> i2 >> i3;
            std::vector<int> i1f = splitFaceStr(i1);
            std::vector<int> i2f = splitFaceStr(i2);
            std::vector<int> i3f = splitFaceStr(i3);
            triangles.push_back(Triangle(i1f[0]-1, i2f[0]-1, i3f[0]-1));            
            std::string i4;
            if(ss >> i4) {
                std::vector<int> i4f = splitFaceStr(i4);
                triangles.push_back(Triangle(i4f[0]-1, i1f[0]-1, i3f[0]-1));
            }
            std::string i5;
            if(ss >> i5) 
                throw std::runtime_error("The object file contains n-gon (n>4) that we do not support.");
        } // Currently ignore other tokens
    }

    std::vector<Normal> normals;
    if(m_ComputeNormals)
        ComputeNormals(vertices, triangles, normals);
    return std::make_shared<TriangleMesh>(vertices, normals, triangles);
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
    normals.resize(vertices.size());

    // Thuermer and Wuethrich, "Computing Vertex Normal from Polygonal Faces"
    // Code borrowed from mitsuba
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
