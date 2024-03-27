#include "ONB.h"

Vector3 ONB::Local(const double a, const double b, const double c) const
{
    return a * U() + b * V() + c * W();
}

Vector3 ONB::Local(const Vector3& a) const
{
    return a.x * U() + a.y * V() + a.z * W();
}

void ONB::BuildFromW(const Vector3& W)
{
    const Vector3 unitW = Unit(W);
    const Vector3 a = (fabs(unitW.x) > 0.9) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    const Vector3 V = Unit(Cross(unitW, a));
    const Vector3 U = Cross(unitW, V);
    axis[0] = U;
    axis[1] = V;
    axis[2] = unitW;
}
