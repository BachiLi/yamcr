#include "random.h"

namespace yamcr {

float RandomSampler::Next1D() {
    return m_Dist(m_Generator);
}

Point2 RandomSampler::Next2D() {
    return Point2(m_Dist(m_Generator), m_Dist(m_Generator));
}

}
