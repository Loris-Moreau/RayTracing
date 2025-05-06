#pragma once

#include <algorithm>
#include <iostream>

#include "Common.h"

class Vector3
{
 public:
    Vector3() : e{0, 0, 0} {}
    
    //Vector3(Vector3&) = default;
    
    Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    Vector3 operator-() const;

    double operator[](int i) const;
    double& operator[](int i);

    Vector3& operator+=(const Vector3& v);
    Vector3& operator*=(const double t);
    Vector3& operator/=(const double t);

    double x() const;
    double y() const;
    double z() const;

    double length() const;
    double length_squared() const;

    void write_color(std::ostream& out, int samples_per_pixel);

    static Vector3 random();
    static Vector3 random(double min, double max);

    double e[3];
};

// Type aliases for Vector3
using point3 = Vector3; // 3D point
using color = Vector3; // RGB color

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
    return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
    return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
    return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline Vector3 operator*(double t, const Vector3& v)
{
    return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vector3 operator*(const Vector3& v, double t)
{
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t)
{
    return (1 / t) * v;
}

inline double dot(const Vector3& u, const Vector3& v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3& u, const Vector3& v)
{
    return {u.e[1] * v.e[2] - u.e[2] * v.e[1],
            u.e[2] * v.e[0] - u.e[0] * v.e[2],
            u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

inline Vector3 unit_vector(Vector3 v)
{
    return v / v.length();
}

inline Vector3 random_in_unit_sphere()
{
    while (true)
    {
        Vector3 p = Vector3::random(-1, 1);
        if (p.length_squared() >= 1)
        {
            continue;
        }

        return p;
    }
}

inline Vector3 random_unit_vector()
{
    const double a = random_double(0, 2 * pi);
    const double z = random_double(-1, 1);
    const double r = sqrt(1 - z * z);

    return Vector3{r * std::cos(a), r * std::sin(a), z};
}

inline Vector3 random_in_hemisphere(const Vector3& normal)
{
    const Vector3 in_unit_sphere = random_in_unit_sphere();

    // In the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0)
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

inline Vector3 reflect(const Vector3& v, const Vector3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat)
{
    const double cos_theta = dot(-uv, n);
    const Vector3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    const Vector3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n;

    return r_out_parallel + r_out_perp;
}

inline Vector3 random_in_unit_disk()
{
    while (true)
    {
        Vector3 p = Vector3{random_double(-1, 1), random_double(-1, 1), 0};

        if (p.length_squared() >= 1)
        {
            continue;
        }

        return p;
    }
}
