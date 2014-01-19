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

#ifndef YAMCR_SAMPLER_H__
#define YAMCR_SAMPLER_H__

#include "vector.h"
#include <memory>

namespace yamcr {

class Sampler {
public:
    virtual std::shared_ptr<Sampler> Clone() = 0;
    virtual float Next1D() = 0;
    virtual Point2 Next2D() = 0;
};

}

#endif //YAMCR_SAMPLER_H__
