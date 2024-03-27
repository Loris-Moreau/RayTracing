#pragma once

#include "Vector3.h"

class ONB
{
public:
    ONB() = default;

    Vector3 operator[](const int i) const { return axis[i]; }
    Vector3& operator[](const int i) { return axis[i]; }

    Vector3 U() const { return axis[0]; }
    Vector3 V() const { return axis[1]; }
    Vector3 W() const { return axis[2]; }

    Vector3 Local(double a, double b, double c) const;

    Vector3 Local(const Vector3& a) const;

    void BuildFromW(const Vector3& W);

public:
    Vector3 axis[3];
};
