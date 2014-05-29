/* 
    The code is borrowed from Mitsuba developed by Wenzel Jakob and others
    http://www.mitsuba-renderer.org/
    Following is the copyright notices from the original file

    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2012 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef YAMCR_SPECTRUM_H__
#define YAMCR_SPECTRUM_H__

#include <array>
#include <iostream>

namespace yamcr {

/**
 *  Abstract generic spectrum class borrowed from mitsuba
 *  http://www.mitsuba-renderer.org/
 */
template <typename T, int N>
struct TSpectrum {    
    enum {
        Dimension = N
    };

    TSpectrum() {}
    
    explicit TSpectrum(T v) {
        for(int i = 0; i < N; i++)
            data[i] = v;
    }
    
    explicit TSpectrum(const std::array<T,N> &spec) {
        data = spec;
    }
    
    template<typename T2> explicit TSpectrum(const TSpectrum<T2, N> &v) {
        for(int i = 0; i < N; i++)
            data[i] = v.data[i];
    }

    TSpectrum operator+(const TSpectrum &spec) const {
        TSpectrum result(*this);
        for(int i = 0; i < N; i++)
            result.data[i] += spec.data[i];
        return result;
    }

    TSpectrum& operator+=(const TSpectrum &spec) {
        for(int i = 0; i < N; i++)
            data[i] += spec.data[i];
        return *this;
    }

    TSpectrum operator-(const TSpectrum &spec) const {
        TSpectrum result(*this);
        for(int i = 0; i < N; i--)
            result.data[i] -= spec.data[i];
        return result;
    }

    TSpectrum& operator-=(const TSpectrum &spec) {
        for(int i = 0; i < N; i--)
            data[i] -= spec.data[i];
        return *this;
    }

    TSpectrum operator*(const TSpectrum &spec) const {
        TSpectrum result(*this);
        for(int i = 0; i < N; i++)
            result.data[i] *= spec.data[i];
        return result;
    }

    TSpectrum& operator*=(const TSpectrum &spec) {
        for(int i = 0; i < N; i++)
            data[i] *= spec.data[i];
        return *this;
    }

    TSpectrum operator*(T v) const {
        TSpectrum result(*this);
        for(int i = 0; i < N; i++)
            result.data[i] *= v;
        return result;
    }

    friend TSpectrum operator*(T v, const TSpectrum &spec) {
        return spec*v;
    }

    TSpectrum& operator*=(T v) {
        for(int i = 0; i < N; i++)
            data[i] *= v;
        return *this;
    }   

    TSpectrum operator/(const TSpectrum &spec) const {
        TSpectrum result(*this);
        for(int i = 0; i < N; i++)
            result.data[i] /= spec.data[i];
        return result;
    }

    TSpectrum& operator/=(const TSpectrum &spec) {
        for(int i = 0; i < N; i++)
            data[i] /= spec.data[i];
        return *this;
    }

    TSpectrum operator/(T v) const {
        TSpectrum result(*this);
        for(int i = 0; i < N; i++)
            result.data[i] /= v;
        return result;
    }

    TSpectrum& operator/=(T v) {
        for(int i = 0; i < N; i++)
            data[i] /= v;
        return *this;
    }   

    bool IsBlack() const {
        for(int i = 0; i < N; i++)
            if(data[i] != 0.f)
                return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream &os, const TSpectrum &s) {
        for(int i = 0; i < N; i++) { 
            os << s.data[i];
            if(i != N-1)
                os << " ";
        }

        return os;
    }

    std::array<T,N> data;
};


struct RGBSpectrum : public TSpectrum<float, 3> {
    RGBSpectrum() {
        data[0] = data[1] = data[2] = 0.f;
    }
    RGBSpectrum(const TSpectrum &spec) {
        data = spec.data;
    }
    explicit RGBSpectrum(float v) {
        data[0] = data[1] = data[2] = v;
    }
    explicit RGBSpectrum(float r, float g, float b) {
        data[0] = r;
        data[1] = g;
        data[2] = b;
    }
    explicit RGBSpectrum(const std::array<float,3> &spec) {
        data = spec;
    }
   
    float Y() const {
        const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
        return YWeight[0] * data[0] + YWeight[1] * data[1] + YWeight[2] * data[2];
    }
};

}

#endif //YAMCR_SPECTRUM_H__
