#pragma once

#include "Hittable.h"
#include "HittableCollection.h"
#include <cmath>

class Quadrilaterals final : public Hittable
{
public:
    Quadrilaterals(const Position& _q, const Vector3& _u, const Vector3& _v, const shared_ptr<Materials>& _mat);

    virtual void SetBoundingBox();

    AABB BoundingBox() const override { return bBox; }

    bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

    virtual bool isInterior(double a, double b, HitInfo& hitInfo) const;

    double PDFValue(const Position& origin, const Vector3& v) const override 
    {
        HitInfo hitInfo;
        if (!this->Hit(Ray(origin, v, 0.0), Interval(0.001, infinity), hitInfo))
        {
            return 0;
        }

        const auto distance_squared = hitInfo.time * hitInfo.time * v.SquaredLength();
        const auto cosine = fabs(Dot(v, hitInfo.normal) / v.Length());

        return distance_squared / (cosine * area);
    }

    Vector3 Random(const Position& origin) const override 
    {
        const Vector3 plane_origin;
        const Vector3 axis_A;
        const Vector3 axis_B;
        const Vector3 position = plane_origin + (RandomDouble() * axis_A) + (RandomDouble() * axis_B);

        return position - origin;
    }
private:
    Position Q;
    Vector3 U, V, W;

    shared_ptr<Materials> mat;
    AABB bBox;

    Vector3 normal;
    double D;

    double area;
};

inline shared_ptr<HittableCollection> Box(const Position& a, const Position& b, const shared_ptr<Materials>& _mat)
{
    //Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    shared_ptr<HittableCollection> sides = make_shared<HittableCollection>();
    
    //Construct the two opposite vertices with the minimum and maximum coordinates.
    const Position min = Position(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    const Position max = Position(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    Vector3 dx = Vector3(max.x - min.x, 0, 0);
    Vector3 dy = Vector3(0, max.y - min.y, 0);
    Vector3 dz = Vector3(0, 0, max.z - min.z);

    sides->Add(make_shared<Quadrilaterals>(Position(min.x, min.y, max.z), dx, dy, _mat)); //front
    sides->Add(make_shared<Quadrilaterals>(Position(max.x, min.y, max.z), -dz, dy, _mat)); //right
    sides->Add(make_shared<Quadrilaterals>(Position(max.x, min.y, min.z), -dx, dy, _mat)); //back
    sides->Add(make_shared<Quadrilaterals>(Position(min.x, min.y, min.z), dz, dy, _mat)); //left
    sides->Add(make_shared<Quadrilaterals>(Position(min.x, max.y, max.z), dx, -dz, _mat)); //top
    sides->Add(make_shared<Quadrilaterals>(Position(min.x, min.y, min.z), dx, dz, _mat)); //bottom

    return sides;
}
