#pragma once
#include "Hittable.h"

class Sphere : public Hittable
{
public:
    // Stationary Sphere
    Sphere(Position _center1, double _radius, shared_ptr<Materials> _material) 
        : mCenter(_center1), mRadius(_radius), mat(_material), isMoving(false) {}

    // Moving Sphere
    Sphere(Position _center2, Position _center3, double _radius, shared_ptr<Materials> _material)
        : mCenter(_center2), mRadius(_radius), mat(_material), isMoving(true) 
    {
        center_vec = _center3 - _center2;
    }

	//Sphere(Position center, double radius, shared_ptr<Materials> material) :mCenter(center), mRadius(radius), mat(material) {}
	bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

	
private:
	Position mCenter;
	double mRadius;

	shared_ptr<Materials> mat;

    bool isMoving;
    Vector3 center_vec;

    Position Center(double time) const;
};
