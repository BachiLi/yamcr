#ifndef YAMCR_TRANSFORM_H__
#define YAMCR_TRANSFORM_H__

#include "vector.h"
#include "ray.h"
#include <Eigen/Dense>

namespace yamcr {

/**
 *  A wrapper of the Eigen Transform class to deal with different transformation applies to
 *  points, vectors and normals
 */
class Transform {
public:
	/// Create the identity transform
	Transform() : 
		m_Transform(Eigen::Matrix4f::Identity()),
		m_Inverse(Eigen::Matrix4f::Identity()) { }

	Transform(const Eigen::Affine3f &trafo)
        : m_Transform(trafo), m_Inverse(trafo.inverse()) {}

	Transform(const Eigen::Affine3f &trafo, const Eigen::Affine3f &inv) 
		: m_Transform(trafo), m_Inverse(inv) { }

	/// Return the inverse transformation
	Transform inverse() const {
		return Transform(m_Inverse, m_Transform);
	}

	/// Concatenate with another transform
	Transform operator*(const Transform &t) const {
        return *this * t;
    }

	/// Apply the homogeneous transformation to a 3D vector
	inline Vector operator*(const Vector &v) const {
		return m_Transform.linear() * v;
	}

	/// Apply the homogeneous transformation to a 3D normal
    /// Notice that the normal is not normalized
    inline Normal operator*(const Normal &n) const {
		return m_Inverse.linear().transpose() * n;
	}

	/// Transform a point by an arbitrary matrix in homogeneous coordinates
	inline Point operator*(const Point &p) const {
        return m_Transform * p;
	}

	/// Apply the homogeneous transformation to a ray
	inline Ray operator*(const Ray &r) const {
		return Ray(
			operator*(r.org), 
			(operator*(r.dir)).normalized(), 
			r.tnear, r.tfar,
            r.time, r.mask
		);
	}

private:
	Eigen::Affine3f m_Transform;
	Eigen::Affine3f m_Inverse;
};

}

#endif /* __TRANSFORM_H */
