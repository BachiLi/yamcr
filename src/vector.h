#ifndef YAMCR_VECTOR_H__
#define YAMCR_VECTOR_H__

#include <ostream>
#include <cmath>

namespace yamcr {

template <typename T> struct TPoint3;
struct VectorA;

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

    TVector3(const VectorA& v);

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

    T LengthSquared() const {
        return x*x+y*y+z*z;
    }

    T Length() const {
        return std::sqrt(LengthSquared());
    }

    friend std::ostream& operator<<(std::ostream& cout, const TVector3 &a) {
        return cout << "(" << a.x << ", " << a.y << ", " << a.z << ")";
    }

    T x, y, z;
};

typedef TVector3<float> Vector;

/**
 *   A 3-d float vector with padding for memory alignment
 */
struct VectorA : public Vector {
    VectorA() {}
    VectorA(float v) : Vector(v), a(0) {}
    VectorA(float x, float y, float z) : Vector(x, y, z), a(0) {}
    VectorA(const Vector &v) : Vector(v), a(0) {}
    template<typename T>
    explicit VectorA(const TPoint3<T> &p) : Vector(p), a(0) {}
    
    int a;
};

template<typename T>
TVector3<T>::TVector3(const VectorA& v) : 
        x((T)v.x), y((T)v.y), z((T)v.z) {}


}

#endif //YAMCR_VECTOR_H__
