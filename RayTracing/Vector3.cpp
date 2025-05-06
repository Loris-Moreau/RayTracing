#include "Vector3.h"

#include "Common.h"

Vector3 Vector3::operator-() const
{
    return Vector3{-e[0], -e[1], -e[2]};
}

double Vector3::operator[](int i) const
{
    return e[i];
}

double& Vector3::operator[](int i)
{
    return e[i];
}

Vector3& Vector3::operator+=(const Vector3& v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

Vector3& Vector3::operator*=(const double t)
{
    e[0] += t;
    e[1] += t;
    e[2] += t;

    return *this;
}

Vector3& Vector3::operator/=(const double t)
{
    return *this *= (1 / t);
}

double Vector3::x() const
{
    return e[0];
}

double Vector3::y() const
{
    return e[1];
}

double Vector3::z() const
{
    return e[2];
}

double Vector3::length() const
{
    return std::sqrt(length_squared());
}

double Vector3::length_squared() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

void Vector3::write_color(std::ostream& out, int samples_per_pixel)
{
    // Divide the color total by the number of samples and gamma-correct
    // for a gamma value of 2.0.
    const double scale = 1.0 / samples_per_pixel;
    const double r = sqrt(scale * e[0]);
    const double g = sqrt(scale * e[1]);
    const double b = sqrt(scale * e[2]);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * std::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * std::clamp(b, 0.0, 0.999)) << '\n';
}

Vector3 Vector3::random()
{
    return Vector3(random_double(), random_double(), random_double());
}

Vector3 Vector3::random(double min, double max)
{
    return Vector3(random_double(min, max), random_double(min, max),
                random_double(min, max));
}
