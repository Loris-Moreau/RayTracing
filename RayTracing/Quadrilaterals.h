#pragma once

#include "Hittable.h"
#include "HittableCollection.h"
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

inline shared_ptr<HittableCollection> box(const Position& a, const Position& b, shared_ptr<Materials> mat)
{
    //Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    shared_ptr<HittableCollection> sides = make_shared<HittableCollection>();
    
    //Construct the two opposite vertices with the minimum and maximum coordinates.
    Position min = Position(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    Position max = Position(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    Vector3 dx = Vector3(max.x - min.x, 0, 0);
    Vector3 dy = Vector3(0, max.y - min.y, 0);
    Vector3 dz = Vector3(0, 0, max.z - min.z);

    sides->Add(make_shared<Quadrilaterals>(Position(min.x, min.y, max.z), dx, dy, mat)); //front
    sides->Add(make_shared<Quadrilaterals>(Position(max.x, min.y, max.z), -dz, dy, mat)); //right
    sides->Add(make_shared<Quadrilaterals>(Position(max.x, min.y, min.z), -dx, dy, mat)); //back
    sides->Add(make_shared<Quadrilaterals>(Position(min.x, min.y, min.z), dz, dy, mat)); //left
    sides->Add(make_shared<Quadrilaterals>(Position(min.x, max.y, max.z), dx, -dz, mat)); //top
    sides->Add(make_shared<Quadrilaterals>(Position(min.x, min.y, min.z), dx, dz, mat)); //bottom

    return sides;
}
