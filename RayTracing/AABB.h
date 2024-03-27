#pragma once

//Axis-aligned bounding box class
#include "Interval.h"

class AABB
{
public:
    Interval x, y, z;

    AABB() = default; // The default AABB is empty, since intervals are empty by default.
    AABB(const Interval& ix, const Interval& iy, const Interval& iz) : x(ix), y(iy), z(iz) {}
    AABB(const Position& a, const Position& b);

    AABB(const AABB& box0, const AABB& box1)
    {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }

    const Interval& axis(int n) const;

    bool Hit(const Ray& ray, Interval rayT) const;

    AABB Pad() const;

};

inline AABB operator+(const AABB& bBox, const Vector3& offset)
{
    return AABB(bBox.x + offset.x, bBox.y + offset.y, bBox.z + offset.z);
}

inline AABB operator+(const Vector3& offset, const AABB& bBox)
{
    return bBox + offset;
}
