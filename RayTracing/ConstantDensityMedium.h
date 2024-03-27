#pragma once

#include "MaterialS.h"

class ConstantDensityMedium final : public Hittable
{
public:
    ConstantDensityMedium(const shared_ptr<Hittable>& b, const double d, const shared_ptr<Texture>& a)
        : boundary(b), negInvDensity(-1 / d), phaseFunction(make_shared<Isotropic>(a)) {}

    ConstantDensityMedium(const shared_ptr<Hittable>& b, const double d, Color color)
        : boundary(b), negInvDensity(-1 / d), phaseFunction(make_shared<Isotropic>(color)) {}

    bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

    AABB BoundingBox() const override
    {
        return boundary->BoundingBox();
    }

private:
    shared_ptr<Hittable> boundary;

    double negInvDensity;

    shared_ptr<Materials> phaseFunction;
};
