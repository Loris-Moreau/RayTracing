#pragma once

#include "AABB.h"

class Materials;

class HitInfo
{
public:
	Position coordinates;
	Vector3 normal;
	
	double time;

	double x;
	double y;

	bool frontFace;

	void SetFaceNormal(const Ray& ray, const Vector3& outwardNormal);
	
	shared_ptr<Materials> mat;
};

class Hittable
{
public:
	Hittable() = default;
	virtual ~Hittable() = default;
	
	virtual bool Hit(const Ray& rRay, Interval rayTime, HitInfo& hitInfo) const = 0;

	virtual AABB BoundingBox() const = 0;

	virtual double PDFValue(const Position& o, const Vector3& v) const;

	virtual Vector3 Random(const Vector3& o) const;
};

class Translate final : public Hittable
{
public:
	Translate(const shared_ptr<Hittable>& position, const Vector3& displacement);

	bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

    AABB BoundingBox() const override { return bBox; }

private:
    shared_ptr<Hittable> object;
    Vector3 offset;
    AABB bBox;
};

class RotateY final : public Hittable
{
public:
	RotateY(const shared_ptr<Hittable>& position, double angle);

	bool Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const override;

	AABB BoundingBox() const override { return bBox; }

private:
	shared_ptr<Hittable> object;
	double sinTheta;
	double cosTheta;
	AABB bBox;
};
