#pragma once

#include <cmath>
#include <iostream>
#include "Utility.h"

class Vector3
{
public:
    double x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(const double pX, const double pY, const double pZ) : x(pX), y(pY), z(pZ) {}
    
    Vector3 operator-() const { return {-x, -y, -z}; }

    double operator[](const int i) const { return i == 0 ? x : (i == 1 ? y : z); }
    double& operator[](const int i) { return i == 0 ? x : (i == 1 ? y : z); }

    Vector3& operator+=(const Vector3& rVec);

    Vector3& operator*=(double t);

    Vector3& operator/=(double t);

    double Length() const;

    double SquaredLength() const;

    bool NearZero() const;

    static Vector3 Random();

    static Vector3 Random(double min, double max);
};

//Alias for Vector3 to increase code readability
using Position = Vector3;

inline std::ostream& operator<<(std::ostream& rOut, const Vector3& rV)
{
    return rOut << rV.x << ' ' << rV.y << ' ' << rV.z << '\n';
}

inline Vector3 operator+(const Vector3& rLeft, const Vector3& rRight)
{
    return {rLeft.x + rRight.x, rLeft.y + rRight.y, rLeft.z + rRight.z};
}

inline Vector3 operator-(const Vector3& rLeft, const Vector3& rRight)
{
    return {rLeft.x - rRight.x, rLeft.y - rRight.y, rLeft.z - rRight.z};
}

inline Vector3 operator*(const Vector3& rLeft, const Vector3& rRight)
{
    return {rLeft.x * rRight.x, rLeft.y * rRight.y, rLeft.z * rRight.z};
}

inline Vector3 operator*(const Vector3& rLeft, const double scalar)
{
    return {rLeft.x * scalar, rLeft.y * scalar, rLeft.z * scalar};
}

inline Vector3 operator*(const double scalar, const Vector3& rRight)
{
    return rRight * scalar;
}

inline Vector3 operator/(const Vector3& vector, const double scalar)
{
    return (1 / scalar) * vector;
}

inline double Dot(const Vector3& rLeft, const Vector3& rRight)
{
    return rLeft.x * rRight.x
        + rLeft.y * rRight.y
        + rLeft.z * rRight.z;
}

inline Vector3 Cross(const Vector3& rLeft, const Vector3& rRight)
{
    return
    {
        rLeft.y * rRight.z - rLeft.z * rRight.y,
        rLeft.z * rRight.x - rLeft.x * rRight.z,
        rLeft.x * rRight.y - rLeft.y * rRight.x
    };
}

inline Vector3 Unit(const Vector3& vector)
{
    return vector / vector.Length();
}

inline Position RandomInUnitSphere() //Las Vegas Algorithm
{
    while (true)
    {
        Position position = Vector3::Random(-1, 1);
        if (position.SquaredLength() < 1) return position;
    }
}

inline Vector3 RandomUnitVector()
{
    return Unit(RandomInUnitSphere());
}

inline Vector3 RandomInUnitDisc()
{
    while (true)
    {
        Vector3 position = Vector3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
        if (position.SquaredLength() < 1)
        {
            return position;
        }
    }
}

inline Vector3 RandomOnHemisphere(const Vector3& normal)
{
    const Vector3 onUnitSphere = RandomUnitVector();
    //If in the same hemisphere as the normal
    if (Dot(onUnitSphere, normal) > 0.0)
    {
        return onUnitSphere;
    }
    return -onUnitSphere;
}

inline Vector3 Reflect(const Vector3& y, const Vector3& n)
{
    return y - 2 * Dot(y, n) * n;
}

inline Vector3 Refract(const Vector3& uv, const Vector3& n, const double etaiOverEtat)
{
    const double cosTheta = fmin(Dot(-uv, n), 1.0);
    const Vector3 rayOutPerp = etaiOverEtat * (uv + cosTheta * n);
    const Vector3 rayOutParallel = -sqrt(fabs(1.0 - rayOutPerp.SquaredLength())) * n;

    return rayOutPerp + rayOutParallel;
}

inline int RandomInt(const int min, const int max)
{
    // Returns a random integer in [min,max].
    return static_cast<int>(RandomDouble(min, max + 1));
}

inline Vector3 RandomCosineDirection()
{
    const double r1 = RandomDouble();
    const double r2 = RandomDouble();

    const double phi = 2 * pi * r1;
    const double x = cos(phi) * sqrt(r2);
    const double y = sin(phi) * sqrt(r2);
    const double z = sqrt(1 - r2);

    return {x, y, z};
}
