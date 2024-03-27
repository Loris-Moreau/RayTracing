#pragma once

#include "Hittable.h"

class Sphere final : public Hittable
{
public:
    // Stationary Sphere
    Sphere(const Position& _center1, const double _radius, const shared_ptr<Materials>& _material) 
        : mCenter(_center1), mRadius(_radius), mat(_material), isMoving(false)
    {
        const Vector3 roundVector = Vector3(mRadius, mRadius, mRadius);
        bBox = AABB(mCenter - roundVector, mCenter + roundVector);
    }

    // Moving Sphere
    Sphere(const Position& _center2, const Position& _center3, const double _radius, const shared_ptr<Materials>& _material)
        : mCenter(_center2), mRadius(_radius), mat(_material), isMoving(true)
    {
        const Vector3 roundVector = Vector3(mRadius, mRadius, mRadius);
        const AABB box1(_center2 - roundVector, _center2 + roundVector);
        const AABB box2(_center3 - roundVector, _center3 + roundVector);

        bBox = AABB(box1, box2);

        center_vec = _center3 - _center2;
    }

	//Sphere(Position center, double radius, shared_ptr<Materials> material) :mCenter(center), mRadius(radius), mat(material) {}
	bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

    AABB BoundingBox() const override { return bBox; }


private:
	Position mCenter;
	double mRadius;

	shared_ptr<Materials> mat;

    bool isMoving;
    Vector3 center_vec;

    Position Center(double time) const;

    AABB bBox;
    static void GetSphereUV(const Position& position, double& U, double& V);
};
