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

#include "random.h"

namespace yamcr {

std::shared_ptr<Sampler> RandomSampler::Clone() {
    std::shared_ptr<RandomSampler> newSampler = 
        std::make_shared<RandomSampler>();
    newSampler->m_Generator = m_Generator;
    newSampler->m_Generator.seed(m_Generator());
    return std::shared_ptr<Sampler>(newSampler);
}

float RandomSampler::Next1D() {
    return m_Dist(m_Generator);
}

Point2 RandomSampler::Next2D() {
    return Point2(m_Dist(m_Generator), m_Dist(m_Generator));
}

}
