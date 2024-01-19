#pragma once

#include "Interval.h"
#include "AABB.h"

class Materials;

class HitInfo
{
public:
	Position coordinates;
	Vector3 normal;
	shared_ptr<Materials> mat;

	double time;
	bool frontFace;

	void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal);
	
	double x;
	double y;
};

class Hittable
{
public:
	virtual ~Hittable() {}
	virtual bool Hit(const Ray& rRay, Interval rayTime, HitInfo& hitInfo) const = 0;

	virtual AABB BoundingBox() const = 0;
};
