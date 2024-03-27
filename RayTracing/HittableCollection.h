#pragma once

#include "Hittable.h"
#include <memory>
#include <vector>
#include "AABB.h"

class HittableCollection final : public Hittable
{
public:
	HittableCollection() = default;
	HittableCollection(const std::shared_ptr<Hittable>& hittable);

	std::vector<std::shared_ptr<Hittable>> mPool;

	void Clear();
	void Add(const std::shared_ptr<Hittable>& hittable);

	bool Hit(const Ray& rRay, Interval rayTime, HitInfo& hitInfo) const override;

	AABB BoundingBox() const override;

private:
	AABB bBox;
};
