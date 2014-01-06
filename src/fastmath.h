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
