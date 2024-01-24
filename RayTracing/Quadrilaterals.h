#pragma once

#include "Hittable.h"
#include <cmath>

class Quadrilaterals : public Hittable
{
public:
    Quadrilaterals(const Position& _Q, const Vector3& _u, const Vector3& _v, shared_ptr<Materials> _mat);

    virtual void SetBoundingBox();

    AABB BoundingBox() const override { return bBox; }

    bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

    virtual bool isInterior(double a, double b, HitInfo& hitInfo) const;

private:
    Position Q;
    Vector3 u, v, w;
    shared_ptr<Materials> mat;
    AABB bBox;

    Vector3 normal;
    double D;
};
