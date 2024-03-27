#include "Interval.h"

bool Interval::Contains(const double x) const 
{ 
	return min <= x && x <= max; 
}

bool Interval::Surrounds(const double x) const
{
    return min < x && x < max;
}

double Interval::Clamp(const double x) const
{
    return x < min ? min : x > max ? max : x;
}

double Interval::Size() const
{
    return max - min;
}

Interval Interval::Expand(const double delta) const
{
    const double padding = delta / 2;
    return {min - padding, max + padding};
}

const Interval Interval::Empty = Interval(+infinity, -infinity);
const Interval Interval::Universe = Interval(-infinity, +infinity);
