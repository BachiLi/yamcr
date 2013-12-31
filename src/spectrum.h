#ifndef YAMCR_SPECTRUM_H__
#define YAMCR_SPECTRUM_H__

#include <array>

namespace yamcr {

/**
 *  Abstract generic spectrum class borrowed from mitsuba
 *  http://www.mitsuba-renderer.org/
 */
template <typename T, int N>
struct TSpectrum {    
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

    std::array<T,N> data;
};

typedef TSpectrum<float, 3> RGBSpectrum;

}

#endif //YAMCR_SPECTRUM_H__
