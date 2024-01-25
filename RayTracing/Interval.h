#pragma once

#include "Ray.h"

class Interval
{
public:
    double min, max;

    Interval() : min(+infinity), max(-infinity) {} //Default interval is empty
    Interval(double pMin, double pMax) : min(pMin), max(pMax) {}
    Interval(const Interval& a, const Interval& b) : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    bool Contains(double x) const;

    bool Surrounds(double x) const;

    double Clamp(double x) const;

    double Size() const;

    Interval Expand(double delta) const;

    static const Interval Empty, Universe;
};

//const static Interval Empty(+infinity, -infinity);
//const static Interval Universe(-infinity, +infinity);

inline Interval operator+(const Interval& interval, double displacement)
{
    return Interval(interval.min + displacement, interval.max + displacement);
}

inline Interval operator+(double displacement, const Interval& interval)
{
    return interval + displacement;
}