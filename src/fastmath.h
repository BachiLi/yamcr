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

#ifndef YAMCR_FASTMATH_H__
#define YAMCR_FASTMATH_H__

#include <emmintrin.h>
#include <xmmintrin.h>
#include "commondefs.h"

namespace yamcr {

/**
 *  Fast math utilities copied from Embree renderer
 */
#ifdef USE_SSE
  __forceinline float Rcp  ( const float x ) 
  { 
    const __m128 vx = _mm_set_ss(x);
    const __m128 r = _mm_rcp_ps(vx);
    return _mm_cvtss_f32(_mm_sub_ps(_mm_add_ps(r, r), _mm_mul_ps(_mm_mul_ps(r, r), vx)));
  }
  __forceinline float Rsqrt( const float x ) { 
    const __m128 a = _mm_set_ss(x);
    const __m128 r = _mm_rsqrt_ps(a);
    const __m128 c = _mm_add_ps(_mm_mul_ps(_mm_set_ps(1.5f, 1.5f, 1.5f, 1.5f), r),
                                _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(a, _mm_set_ps(-0.5f, -0.5f, -0.5f, -0.5f)), r), _mm_mul_ps(r, r)));
    return _mm_cvtss_f32(c);
  }
#else
  __forceinline float Rcp  ( const float x ) { return 1.0f/x; }
  __forceinline float Rsqrt( const float x ) { return 1.0f/sqrtf(x); }
#endif

}
#endif //YAMCR_FASTMATH_H__
