#ifndef YAMCR_POINT_H__
#define YAMCR_POINT_H__

#include "vector.h"

namespace yamcr {

/**
 * Generic 3-d point borrowed from Mitsuba's implementation
 * http://www.mitsuba-renderer.org/
 */
template<typename T>
struct TPoint3 {
    TPoint3() {}
    
    TPoint3(T x, T y, T z) :
        x(x), y(y), z(z) {}
    
    explicit TPoint3(T v) :
        x(v), y(v), z(v) {}
    
    template <typename T2> explicit TPoint3(const TPoint3<T2> &p) :
        x((T)p.x), y((T)p.y), z((T)p.z) {}

    template <typename T2> explicit TPoint3(const TVector3<T2> &v) :
        x((T)v.x), y((T)v.y), z((T)v.z) {}

    TPoint3 operator+(const TVector3<T> &v) const {
        return TPoint3(x + v.x, y + v.y, z + v.z);
    }

    TPoint3& operator+=(const TVector3<T> &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    TPoint3 operator+(const TPoint3 &v) const {
        return TPoint3(x + v.x, y + v.y, z + v.z);
    }

    TPoint3& operator+=(const TPoint3 &v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    TPoint3 operator+(T v) const {
        return TPoint3(x + v, y + v, z + v);
    }

    TPoint3& operator+=(T v) {
        x += v; y += v; z += v;
        return *this;
    }

    friend TPoint3 operator+(float v1, const TPoint3 &v2) {
        return v2+v1;
    }

    TPoint3 operator-(const TVector3<T> &v) const {
        return TPoint3(x - v.x, y - v.y, z - v.z);
    }

    TPoint3& operator-=(const TVector3<T> &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    TPoint3 operator-(const TPoint3 &v) const {
        return TPoint3(x - v.x, y - v.y, z - v.z);
    }

    TPoint3& operator-=(const TPoint3 &v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    TPoint3 operator-(T v) const {
        return TPoint3(x - v, y - v, z - v);
    }

    TPoint3& operator-=(T v) {
        x -= v; y -= v; z -= v;
        return *this;
    }

    friend TPoint3 operator-(float v1, const TPoint3 &v2) {
        return v2-v1;
    }

    TPoint3 operator*(T v) const {
        return TPoint3(x * v, y * v, z * v);
    }

    TPoint3& operator*=(T v) {
        x *= v; y *= v; z *= v;
        return *this;
    }

    friend TPoint3 operator*(float v1, const TPoint3 &v2) {
        return v2*v1;
    }

    TPoint3 operator-() {
        return TPoint3(-x, -y, -z);
    }

    TPoint3 operator/(T v) const {
        T invV = (T)1 / v;
        return TPoint3(x*invV, y*invV, z*invV);
    }

    TPoint3& operator/=(T v) {
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

    friend std::ostream& operator<<(std::ostream& cout, const TPoint3 &a) {
        return cout << "(" << a.x << ", " << a.y << ", " << a.z << ")";
    }

    T x, y, z;
};

typedef TPoint3<float> Point;

}

#endif //YAMCR_POINT_H__
