#pragma once
#include "Hittable.h"
#include <memory>
#include <vector>
#include "AABB.h"

class HittableCollection : public Hittable
{
public:
	HittableCollection() = default;
	HittableCollection(std::shared_ptr<Hittable> hittable) { Add(hittable); }

	std::vector<std::shared_ptr<Hittable>> mPool;

	void Clear() { mPool.clear(); }
	void Add(std::shared_ptr<Hittable> hittable)
	{
		mPool.push_back(hittable);
		bBox = AABB(bBox, hittable->BoundingBox());
	}

	bool Hit(const Ray& rRay, Interval rayTime, HitInfo& hitInfo) const override;

	AABB BoundingBox() const override { return bBox; }

private:
	AABB bBox;
};
