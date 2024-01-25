#include "Interval.h"

bool Interval::Contains(double x) const 
{ 
	return min <= x && x <= max; 
}

bool Interval::Surrounds(double x) const
{
    return min < x && x < max;
}

double Interval::Clamp(double x) const
{
    return x < min ? min : x > max ? max : x;
}

double Interval::Size() const
{
    return max - min;
}

Interval Interval::Expand(double delta) const
{
    double padding = delta / 2;
    return Interval(min - padding, max + padding);
}

const Interval Interval::Empty = Interval(+infinity, -infinity);
const Interval Interval::Universe = Interval(-infinity, +infinity);
