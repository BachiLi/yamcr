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

#ifndef YAMCR_SAMPLERS_RANDOM_H__
#define YAMCR_SAMPLERS_RANDOM_H__

#include "sampler.h"
#include <random>

namespace yamcr {

class RandomSampler : public Sampler {
public:
    std::shared_ptr<Sampler> Clone();
    float Next1D();
    Point2 Next2D();
private:
    std::mt19937 m_Generator;
    std::uniform_real_distribution<float> m_Dist;
};

}

#endif //YAMCR_SAMPLER_RANDOM_H__
