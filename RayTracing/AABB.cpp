#include "AABB.h"

AABB::AABB(const Position& a, const Position& b)
{
    // Treat the two points a and b as extrema for the bounding box, so we don't require a particular minimum/maximum coordinate order.
    x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
    y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
    z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
}

const Interval& AABB::axis(int n) const
{
    if (n == 1) 
    {
        return y;
    }
    if (n == 2)
    {
        return z;
    }
    return x;
}

bool AABB::Hit(const Ray& ray, Interval rayT) const
{
    for (int a = 0; a < 3; a++)
    {
        double invD = 1 / ray.GetDirection()[a];
        double origin = ray.GetOrigin()[a];

        double t0 = (axis(a).min - origin) * invD;
        double t1 = (axis(a).max - origin) * invD;

        if (invD < 0)
        {
            std::swap(t0, t1);
        }
        if (t0 > rayT.min)
        {
            rayT.min = t0;
        }
        if (t1 < rayT.max)
        {
            rayT.max = t1;
        }
        if (rayT.max <= rayT.min)
        {
            return false;
        }
    }
    return true;
}