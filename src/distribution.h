/* 
    Some of the code is borrowed from pbrt developed by Matt Pharr and Greg Humphreys
    http://www.pbrt.org/
    Following is the copyright notices from the original file

    pbrt source code Copyright(c) 1998-2012 Matt Pharr and Greg Humphreys.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef YAMCR_DISTRIBUTION_H__
#define YAMCR_DISTRIBUTION_H__

#include "vector.h"
#include "utils.h"
#include <cassert>

namespace yamcr {

class PiecewiseConstant2D;

/**
 *  A class mimics a 1D piece-wise constant distribution, completely borrowed from pbrt
 */
class PiecewiseConstant1D {
public:    
    // PiecewiseConstant1D Public Methods
    PiecewiseConstant1D(const float *f, int n) {
        count = n;
        func = new float[n];
        memcpy(func, f, n*sizeof(float));
        cdf = new float[n+1];
        // Compute integral of step function at $x_i$
        cdf[0] = 0.;
        for (int i = 1; i < count+1; ++i)
            cdf[i] = cdf[i-1] + func[i-1] / n;

        // Transform step function integral into CDF
        funcInt = cdf[count];
        if (funcInt == 0.f) {
            for (int i = 1; i < n+1; ++i)
                cdf[i] = float(i) / float(n);
        }
        else {
            for (int i = 1; i < n+1; ++i)
                cdf[i] /= funcInt;
        }
    }
    ~PiecewiseConstant1D() {
        delete[] func;
        delete[] cdf;
    }
    float SampleContinuous(float u, float *pdf, int *off = NULL) const {
        // Find surrounding CDF segments and _offset_
        float *ptr = std::upper_bound(cdf, cdf+count+1, u);
        int offset = Clamp(int(ptr-cdf-1), 0, count-1);
        if (off) *off = offset;
        assert(u >= cdf[offset] && u < cdf[offset+1]);

        // Compute offset along CDF segment
        float du = (u - cdf[offset]) / (cdf[offset+1] - cdf[offset]);
        assert(!isnan(du));

        // Compute PDF for sampled offset
        if (pdf) *pdf = func[offset] / funcInt;

        // Return $x\in{}[0,1)$ corresponding to sample
        return (offset + du) / count;
    }
    int SampleDiscrete(float u, float *pdf) const {
        // Find surrounding CDF segments and _offset_
        float *ptr = std::upper_bound(cdf, cdf+count+1, u);
        int offset = Clamp(int(ptr-cdf-1), 0, count-1);
        assert(u >= cdf[offset] && u < cdf[offset+1]);
        if (pdf) *pdf = func[offset] / (funcInt * count);
        return offset;
    }
private:
    // PiecewiseConstant1D Private Data
    float *func, *cdf;
    float funcInt;
    int count;
    friend PiecewiseConstant2D;
};

/**
 *  A class mimics a 1D piece-wise constant distribution, completely borrowed from pbrt
 */
class PiecewiseConstant2D {
public:    
    // PiecewiseConstant2D Public Methods
    PiecewiseConstant2D(const float *data, int nu, int nv) {
        pConditionalV.reserve(nv);
        for (int v = 0; v < nv; ++v) {
            // Compute conditional sampling distribution for $\tilde{v}$
            pConditionalV.push_back(new PiecewiseConstant1D(&data[v*nu], nu));
        }
        // Compute marginal sampling distribution $p[\tilde{v}]$
        std::vector<float> marginalFunc;
        marginalFunc.reserve(nv);
        for (int v = 0; v < nv; ++v)
            marginalFunc.push_back(pConditionalV[v]->funcInt);
        pMarginal = new PiecewiseConstant1D(&marginalFunc[0], nv);
    }
    ~PiecewiseConstant2D() {
        delete pMarginal;
        for (uint32_t i = 0; i < pConditionalV.size(); ++i)
            delete pConditionalV[i];
    }
    Point2 SampleContinuous(const Point2 &sample, float *pdf) const {
        float pdfs[2];
        int v;
        Point2 uv;
        uv[1] = pMarginal->SampleContinuous(sample[1], &pdfs[1], &v);
        uv[0] = pConditionalV[v]->SampleContinuous(sample[0], &pdfs[0]);
        *pdf = pdfs[0] * pdfs[1];
        return uv;
    }
    float Pdf(float u, float v) const {
        int iu = Clamp((int)(u * pConditionalV[0]->count), 0,
                       pConditionalV[0]->count-1);
        int iv = Clamp((int)(v * pMarginal->count), 0,
                       pMarginal->count-1);
        if (pConditionalV[iv]->funcInt * pMarginal->funcInt == 0.f) return 0.f;
        return (pConditionalV[iv]->func[iu] * pMarginal->func[iv]) /
               (pConditionalV[iv]->funcInt * pMarginal->funcInt);
    }
private:
    // PiecewiseConstant2D Private Data
    std::vector<PiecewiseConstant1D *> pConditionalV;
    PiecewiseConstant1D *pMarginal;
};




}

#endif //YAMCR_DISTRIBUTION_H__
