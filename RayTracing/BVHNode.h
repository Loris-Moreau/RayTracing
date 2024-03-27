#pragma once

#include "HittableCollection.h"

#include <algorithm>

class BVHNode final : public Hittable 
{
public:
    explicit BVHNode(const HittableCollection& list) : BVHNode(list.mPool, 0, list.mPool.size()) {}

    BVHNode(const std::vector<shared_ptr<Hittable>>& srcObjects, size_t start, size_t end);

    bool Hit(const Ray& ray, const Interval rayT, HitInfo& hitinfo) const override 
    {
        if (!bBox.Hit(ray, rayT))
            return false;

        const bool hit_left = left->Hit(ray, rayT, hitinfo);
        const bool hit_right = right->Hit(ray, Interval(rayT.min, hit_left ? hitinfo.time : rayT.max), hitinfo);

        return hit_left || hit_right;
    }

    AABB BoundingBox() const override { return bBox; }

private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB bBox;

    static bool BoxCompare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b, const int axisIndex)
    {
        return a->BoundingBox().axis(axisIndex).min < b->BoundingBox().axis(axisIndex).min;
    }

    static bool BoxXCompare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b)
    {
        return BoxCompare(a, b, 0);
    }

    static bool BoxYCompare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b)
    {
        return BoxCompare(a, b, 1);
    }

    static bool BoxZCompare(const shared_ptr<Hittable>& a, const shared_ptr<Hittable>& b)
    {
        return BoxCompare(a, b, 2);
    }
};
