#pragma once
#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(Position center, double radius, shared_ptr<Materials> material) :mCenter(center), mRadius(radius), mat(material){}
	bool Hit(const Ray& rRay, Interval rayTime, HitInfo& hitInfo) const override;

	
private:
	Position mCenter;
	double mRadius;

	shared_ptr<Materials> mat;
};
