#ifndef YAMCR_VECTOR_H__
#define YAMCR_VECTOR_H__

#include <ostream>

namespace yamcr {

template <typename T> struct TPoint3;

/**
 * Generic 3-d vector borrowed from Mitsuba's implementation
 * http://www.mitsuba-renderer.org/
 */
template<typename T>
struct TVector3 {
    TVector3() {}
    
    TVector3(T x, T y, T z) :
        x(x), y(y), z(z) {}
    
    explicit TVector3(T v) :
        x(v), y(v), z(v) {}
    
    template <typename T2> explicit TVector3(const TVector3<T2> &v) :
        x((T)v.x), y((T)v.y), z((T)v.z) {}

    template <typename T2> explicit TVector3(const TPoint3<T2> &p) :
        x((T)p.x), y((T)p.y), z((T)p.z) {}

    TVector3 operator+(const TVector3 &v) const {
        return TVector3(x + v.x, y + v.y, z + v.z);
    }

    TVector3& operator+=(const TVector3 &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    TVector3 operator+(T v) const {
        return TVector3(x + v, y + v, z + v);
    }

    TVector3& operator+=(T v) {
        x += v; y += v; z += v;
        return *this;
    }

    friend TVector3 operator+(float v1, const TVector3 &v2) {
        return v2+v1;
    }

    TVector3 operator-(const TVector3 &v) const {
        return TVector3(x - v.x, y - v.y, z - v.z);
    }

    TVector3& operator-=(const TVector3 &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    TVector3 operator-(T v) const {
        return TVector3(x - v, y - v, z - v);
    }

    TVector3& operator-=(float &v) {
        x -= v; y -= v; z -= v;
        return *this;
    }

    friend TVector3 operator-(float v1, const TVector3 &v2) {
        return v2-v1;
    }

    TVector3 operator*(const TVector3 &v) const {
        return TVector3(x * v.x, y * v.y, z * v.z);
    }

    TVector3& operator*=(const TVector3 &v) {
        x *= v.x; y *= v.y; z *= v.z;
        return *this;
    }

    TVector3 operator*(T v) const {
        return TVector3(x * v, y * v, z * v);
    }

    TVector3& operator*=(T v) {
        x *= v; y *= v; z *= v;
        return *this;
    }

    friend TVector3 operator*(float v1, const TVector3 &v2) {
        return v2*v1;
    }

    TVector3 operator-() {
        return TVector3(-x, -y, -z);
    }

    TVector3 operator/(const TVector3 &v) const {
        return TVector3(x/v.x, y/v.y, z/v.z);
    }

    TVector3& operator/=(const TVector3 &v) {
        x /= v.x; y /= v.y; z /= v.z;
        return *this;
    }

    TVector3 operator/(T v) const {
        T invV = (T)1 / v;
        return TVector3(x*invV, y*invV, z*invV);
    }

    TVector3& operator/=(T v) {
        T invV = (T)1 / v;
        x *= invV; y *= invV; z *= invV;
        return *this;
    }    

    T& operator[](int i) {
        return (&x)[i];
    }

    T& operator[](int i) const {
        return (&x)[i];
    }

    friend std::ostream& operator<<(std::ostream& cout, const TVector3 &a) {
        return cout << "(" << a.x << ", " << a.y << ", " << a.z << ")";
    }

    T x, y, z;
};

typedef TVector3<float> Vector;

}

#endif //YAMCR_VECTOR_H__
