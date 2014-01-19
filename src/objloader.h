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

#ifndef YAMCR_OBJLOADER_H__
#define YAMCR_OBJLOADER_H__

#include "shapes/trianglemesh.h"
#include "transform.h"

#include <memory>
#include <string>

namespace yamcr {

class ObjLoader {
public:
    ObjLoader(bool computeNormals) :
        m_ComputeNormals(computeNormals) {}
    std::shared_ptr<TriangleMesh> Load(
            const Transform &obj2world, const std::string &filename);
private:
    void ComputeNormals(const std::vector<PointA> &vertices, const std::vector<Triangle> &triangles, 
            std::vector<Normal> &normals);

    bool m_ComputeNormals;
};

}

#endif //YAMCR_OBJLOADER_H__
