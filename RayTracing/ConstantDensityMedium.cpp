#include "ConstantDensityMedium.h"

bool ConstantDensityMedium::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
    //Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && RandomDouble() < 0.00001;

    HitInfo hitInfo1, hitInfo2;

    if (!boundary->Hit(ray, Interval::Universe, hitInfo1))
        return false;

    if (!boundary->Hit(ray, Interval(hitInfo1.time + 0.0001, infinity), hitInfo2))
        return false;

    if (debugging) std::clog << "\nray time min = " << hitInfo1.time << ", ray time max = " << hitInfo2.time << '\n';

    if (hitInfo1.time < rayTime.min) hitInfo1.time = rayTime.min;
    if (hitInfo2.time > rayTime.max) hitInfo2.time = rayTime.max;

    if (hitInfo1.time >= hitInfo2.time)
        return false;

    if (hitInfo1.time < 0)
        hitInfo1.time = 0;

    double rayLength = ray.GetDirection().Length();
    double distanceInsideBoundary = (hitInfo2.time - hitInfo1.time) * rayLength;
    double hitDistance = negInvDensity * log(RandomDouble());

    if (hitDistance > distanceInsideBoundary)
        return false;

    hitInfo.time = hitInfo1.time + hitDistance / rayLength;
    hitInfo.coordinates = ray.At(hitInfo.time);

    if (debugging)
    {
        std::clog << "hitDistance = " << hitDistance << '\n'
            << "hitInfo.time = " << hitInfo.time << '\n'
            << "hitInfo.coordinates = " << hitInfo.coordinates << '\n';
    }

    hitInfo.normal = Vector3(1, 0, 0);  // arbitrary
    hitInfo.frontFace = true;     // also arbitrary
    hitInfo.mat = phaseFunction;

    return true;
}
