#ifndef YAMCR_GEOMETRY_VECTOR_H__
#define YAMCR_GEOMETRY_VECTOR_H__

#include <Eigen/Dense>

// Code shamelessly copied from Nori
// http://www.cs.cornell.edu/Courses/cs6630/2012sp/nori/

namespace yamcr {

/* ===================================================================
    This file contains a few templates and specializations that
    provide 2/3D points, vectors, and normals over different
    underlying data types. Points, vectors, and normals are distinct
    in Nori, because they transform differently under homogeneous
    coordinate transformations.
 * =================================================================== */

template <typename Scalar, int Dimension>  struct TVector;
template <typename Scalar, int Dimension>  struct TPoint;

/**
 * \brief Generic N-dimensional vector data structure based on Eigen::Matrix
 */
template <typename _Scalar, int _Dimension> struct TVector : public Eigen::Matrix<_Scalar, _Dimension, 1> {
public:
	enum {
		Dimension = _Dimension
	};

	typedef _Scalar                             Scalar;
	typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
	typedef TVector<Scalar, Dimension>          VectorType;
	typedef TPoint<Scalar, Dimension>           PointType;

	/// Create a new vector with constant component vlaues
	inline TVector(Scalar value = (Scalar) 0) { this->setConstant(value); }

	/// Create a new 2D vector (type error if \c Dimension != 2)
	inline TVector(Scalar x, Scalar y) : Base(x, y) { }
	
	/// Create a new 3D vector (type error if \c Dimension != 3)
	inline TVector(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

	/// Create a new 4D vector (type error if \c Dimension != 4)
	inline TVector(Scalar x, Scalar y, Scalar z, Scalar w) : Base(x, y, z, w) { }

	/// Construct a vector from MatrixBase (needed to play nice with Eigen)
	template <typename Derived> inline TVector(const Eigen::MatrixBase<Derived>& p) 
		: Base(p) { }

	/// Assign a vector from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> TVector &operator=(const Eigen::MatrixBase<Derived>& p) {
		this->Base::operator=(p);
		return *this;
    }

};

/**
 * \brief Generic N-dimensional point data structure based on Eigen::Matrix
 */
template <typename _Scalar, int _Dimension> struct TPoint : public Eigen::Matrix<_Scalar, _Dimension, 1> {
public:
	enum {
		Dimension = _Dimension
	};

	typedef _Scalar                             Scalar;
	typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
	typedef TVector<Scalar, Dimension>          VectorType;
	typedef TPoint<Scalar, Dimension>           PointType;

	/// Create a new point with constant component vlaues
	inline TPoint(Scalar value = (Scalar) 0) { this->setConstant(value); }

	/// Create a new 2D point (type error if \c Dimension != 2)
	inline TPoint(Scalar x, Scalar y) : Base(x, y) { }
	
	/// Create a new 3D point (type error if \c Dimension != 3)
	inline TPoint(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

	/// Create a new 4D point (type error if \c Dimension != 4)
	inline TPoint(Scalar x, Scalar y, Scalar z, Scalar w) : Base(x, y, z, w) { }

	/// Construct a point from MatrixBase (needed to play nice with Eigen)
	template <typename Derived> inline TPoint(const Eigen::MatrixBase<Derived>& p) 
		: Base(p) { }

	/// Assign a point from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> TPoint &operator=(const Eigen::MatrixBase<Derived>& p) {
		this->Base::operator=(p);
		return *this;
    }

};

/**
 * \brief 3-dimensional surface normal representation
 */
struct Normal3f : public Eigen::Matrix<float, 3, 1> {
public:
	enum {
		Dimension = 3
	};

	typedef float                               Scalar;
	typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
	typedef TVector<Scalar, Dimension>          VectorType;
	typedef TPoint<Scalar, Dimension>           PointType;


	/// Create a new normal with constant component vlaues
	inline Normal3f(Scalar value = 0.0f) { this->setConstant(value); }

	/// Create a new 3D normal 
	inline Normal3f(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

	/// Construct a normal from MatrixBase (needed to play nice with Eigen)
	template <typename Derived> inline Normal3f(const Eigen::MatrixBase<Derived>& p) 
		: Base(p) { }

	/// Assign a normal from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> Normal3f &operator=(const Eigen::MatrixBase<Derived>& p) {
		this->Base::operator=(p);
		return *this;
    }
};

/**
 * \brief 3-dimensional point representation with padding for better alignment
 */
struct PointA : public Eigen::Matrix<float, 3, 1> {
public:
	enum {
		Dimension = 3
	};

	typedef float                               Scalar;
	typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
	typedef TVector<Scalar, Dimension>          VectorType;
	typedef TPoint<Scalar, Dimension>           PointType;


	/// Create a new point with constant component vlaues
	inline PointA(Scalar value = 0.0f) { this->setConstant(value); }

	/// Create a new 3D point
	inline PointA(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

	/// Construct a normal from MatrixBase (needed to play nice with Eigen)
	template <typename Derived> inline PointA(const Eigen::MatrixBase<Derived>& p) 
		: Base(p) { }

	/// Assign a normal from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> PointA &operator=(const Eigen::MatrixBase<Derived>& p) {
		this->Base::operator=(p);
		return *this;
    }
private:
    int pad;
};

/**
 * \brief 3-dimensional vector representation with padding for better alignment
 */
struct VectorA : public Eigen::Matrix<float, 3, 1> {
public:
	enum {
		Dimension = 3
	};

	typedef float                               Scalar;
	typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
	typedef TVector<Scalar, Dimension>          VectorType;
	typedef TPoint<Scalar, Dimension>           PointType;


	/// Create a new point with constant component vlaues
	inline VectorA(Scalar value = 0.0f) { this->setConstant(value); }

	/// Create a new 3D point
	inline VectorA(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

	/// Construct a normal from MatrixBase (needed to play nice with Eigen)
	template <typename Derived> inline VectorA(const Eigen::MatrixBase<Derived>& p) 
		: Base(p) { }

	/// Assign a normal from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> VectorA &operator=(const Eigen::MatrixBase<Derived>& p) {
		this->Base::operator=(p);
		return *this;
    }
private:
    int pad;
};

/**
 * \brief 3-dimensional surface normal representation with padding for better alignment
 */
struct NormalA : public Eigen::Matrix<float, 3, 1> {
public:
	enum {
		Dimension = 3
	};

	typedef float                               Scalar;
	typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
	typedef TVector<Scalar, Dimension>          VectorType;
	typedef TPoint<Scalar, Dimension>           PointType;


	/// Create a new normal with constant component vlaues
	inline NormalA(Scalar value = 0.0f) { this->setConstant(value); }

	/// Create a new 3D normal 
	inline NormalA(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

	/// Construct a normal from MatrixBase (needed to play nice with Eigen)
	template <typename Derived> inline NormalA(const Eigen::MatrixBase<Derived>& p) 
		: Base(p) { }

	/// Assign a normal from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> NormalA &operator=(const Eigen::MatrixBase<Derived>& p) {
		this->Base::operator=(p);
		return *this;
    }
private:
    int pad;
};

typedef TVector<float, 3> Vector3;
typedef Vector3 Vector;

typedef TPoint<float, 2> Point2;
typedef TPoint<int, 2> Point2i;
typedef TPoint<float, 3> Point3;
typedef TPoint<float, 4> Point4;
typedef Point3 Point;

typedef Normal3f Normal;

}

#endif //YAMCR_GEOMETRY_VECTOR_H__
