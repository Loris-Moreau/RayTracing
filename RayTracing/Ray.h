#pragma once

#include "Vector3.h"

class Ray
{
public:
    Ray() = default;
    Ray(const Position& origin, const Vector3& direction) : mOrigin(origin), mDirection(direction), mTime(0) {}
    Ray(const Position& origin, const Vector3& direction, const double time = 0.0) : mOrigin(origin), mDirection(direction), mTime(time) {}

    Position GetOrigin() const { return mOrigin; }
    Vector3 GetDirection() const { return mDirection; }
    double time() const { return mTime; }
    Position At(const double time) const { return mOrigin + time * mDirection; }

private:
    Position mOrigin;
    Vector3 mDirection;
    double mTime;
};
