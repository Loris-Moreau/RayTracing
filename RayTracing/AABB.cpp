#include "AABB.h"

/*
AABB::AABB(const Position& a, const Position& b)
{
    // Treat the two points a and b as extrema for the bounding box, so we don't require a particular minimum/maximum coordinate order.
    x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
    y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
    z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
}
*/
// Opti Constructor Test
AABB::AABB(const Position& a, const Position& b)
       : x(fmin(a.x, b.x), fmax(a.x, b.x)),
         y(fmin(a.y, b.y), fmax(a.y, b.y)),
         z(fmin(a.z, b.z), fmax(a.z, b.z))
{}
// End of Opti Constructor Test

const Interval& AABB::axis(int n) const
{
    /*
    if (n == 1) 
    {
        return y;
    }
    if (n == 2)
    {
        return z;
    }
    return x;
    */
    return (n == 1) ? y : (n == 2) ? z : x;
}

/*
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
*/
// Optimized Hit Func Test
bool AABB::Hit(const Ray& ray, Interval rayT) const
{
    Vector3 invD = ray.GetDirection();  
    invD /= 1.0; // Precompute inverse direction
    
    Vector3 origin = ray.GetOrigin();

    for (int a = 0; a < 3; a++)
    {
        double t0 = (axis(a).min - origin[a]) * invD[a];
        double t1 = (axis(a).max - origin[a]) * invD[a];

        if (invD[a] < 0) std::swap(t0, t1);

        rayT.min = fmax(rayT.min, t0);
        rayT.max = fmin(rayT.max, t1);

        if (rayT.max <= rayT.min) return false;
    }
    return true;
}

// End of Optimized Hit Func Test

/*
AABB AABB::Pad()
{
    // Return an AABB that has no side narrower than some delta, padding if necessary.
    double delta = 0.0001;
    Interval newX = (x.Size() >= delta) ? x : x.Expand(delta);
    Interval newY = (y.Size() >= delta) ? y : y.Expand(delta);
    Interval newZ = (z.Size() >= delta) ? z : z.Expand(delta);

    return AABB(newX, newY, newZ);
}
*/
// Optimized Pad Func Test
AABB AABB::Pad()
{
    constexpr double delta = 0.0001;

    return AABB
    (
        x.Size() >= delta ? x : x.Expand(delta),
        y.Size() >= delta ? y : y.Expand(delta),
        z.Size() >= delta ? z : z.Expand(delta)
    );
}
// End of Optimized Pad Func Test
