#include "intersection.h"
#include "fastmath.h"

namespace yamcr {

// Faster than Eigen's solver
bool SolveLinearSystem2x2(
        float invDet,
        const Eigen::Matrix2f &A,
        const Eigen::Vector2f &B, Eigen::Vector2f &x) {
    x(0) = (A(1, 1)*B(0) - A(0, 1)*B(1)) * invDet;
    x(1) = (A(0, 0)*B(1) - A(1, 0)*B(0)) * invDet;
    if (isnan(x(0)) || isnan(x(1)))
        return false;
    return true;
}

void Intersection::ComputeTextureDifferential(const RayDifferential &rayDiff) {    
    Eigen::Matrix<float, 2, 2> A;
    int axes[2];
    if (std::abs(Ng[0]) > std::abs(Ng[1]) && std::abs(Ng[0]) > std::abs(Ng[2])) {
        axes[0] = 1; axes[1] = 2;
    }
    else if (std::abs(Ng[1]) > std::abs(Ng[2])) {
        axes[0] = 0; axes[1] = 2;
    }
    else {
        axes[0] = 0; axes[1] = 1;
    }
    A(0, 0) = dPds(axes[0]);
    A(1, 0) = dPds(axes[1]);
    A(0, 1) = dPdt(axes[0]);
    A(1, 1) = dPdt(axes[1]);
    Eigen::Vector2f Bx, By;
    Bx(0) = rayDiff.dPdx(axes[0]);
    Bx(1) = rayDiff.dPdx(axes[1]);
    By(0) = rayDiff.dPdy(axes[0]);
    By(1) = rayDiff.dPdy(axes[1]);
    float det = A.determinant();
    if (fabsf(det) < 1e-10f) {
        dSTdx = Vector2(0.f);
        dSTdy = Vector2(0.f);
        return;
    }

    float invDet = Rcp(det);
    if (!SolveLinearSystem2x2(invDet, A, Bx, dSTdx)) {
        dSTdx = Vector2(0.f);
    }
    if (!SolveLinearSystem2x2(invDet, A, By, dSTdy)) {
        dSTdy = Vector2(0.f);
    }
}

}
