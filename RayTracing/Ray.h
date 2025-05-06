#pragma once

#include "Vector3.h"

class Ray
{
 public:
    Ray() = default;

    Ray(const Vector3& origin, const Vector3& direction, double time = 0.0) : orig(origin), dir(direction), tm(time) {}
    
    Vector3 origin() const
    {
        return orig;
    }
    
    Vector3 direction() const
    {
        return dir;
    }
    
    double time() const
    {
        return tm;
    }
    
    Vector3 at(double t) const
    {
        return orig + t * dir;
    }
    
    Vector3 orig;
    Vector3 dir;
    double tm;
};
