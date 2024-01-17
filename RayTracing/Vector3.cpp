#include "Vector3.h"

Vector3& Vector3::operator+=(const Vector3& rVec)
{
    x += rVec.x; y += rVec.y; z += rVec.z;
    return *this;
}

Vector3& Vector3::operator*=(double t)
{
    x *= t; y *= t; z *= t;
    return *this;
}

Vector3& Vector3::operator/=(double t)
{
    x /= t; y /= t; z /= t;
    return *this;
}

double Vector3::Length() const
{
    return sqrt(SquaredLength());
}

double Vector3::SquaredLength() const
{
    return x * x + y * y + z * z;
}

bool Vector3::NearZero() const
{
    // Return true if the vector is close to zero in all dimensions.
    double s = 1e-8;
    return fabs(x < s) && fabs(y < s) && fabs(z < s);
}

Vector3 Vector3::Random()
{
    return Vector3(RandomDouble(), RandomDouble(), RandomDouble());
}

Vector3 Vector3::Random(double min, double max)
{
    return Vector3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
}
