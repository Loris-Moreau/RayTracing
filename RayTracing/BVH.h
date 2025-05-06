#pragma once

#include <algorithm>

#include "AABB.h"
#include "Hittable.h"
#include "HittableList.h"

class BVHNode final : public Hittable
{
 public:
    BVHNode() = default;
    BVHNode(HittableList& list, double time0, double time1)
        : BVHNode(list.objects, 0, list.objects.size(), time0, time1) {}
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end, double time0, double time1);

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& output_box) const override;

    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    
    AABB Box;
};

inline bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    {
        std::cerr << "No Bounding Box in BVHNode constructor.\n";
    }

    return box_a.min().e[axis] < box_b.min().e[axis];
}

inline bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 0);
}

inline bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 1);
}

inline bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
    return box_compare(a, b, 2);
}

inline BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, std::size_t start, std::size_t end, double time0, double time1)
{
    const int axis = random_int(0, 2);
    
    const auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;
    
    const size_t object_span = end - start;

    if (object_span == 1)
    {
        left = right = objects[start];
    }
    else if (object_span == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        unsigned long long mid = start + object_span / 2;
        left = std::make_shared<BVHNode>(objects, start, mid, time0, time1);
        right = std::make_shared<BVHNode>(objects, mid, end, time0, time1);
    }

    AABB box_left, box_right;

    if (!left->bounding_box(time0, time1, box_left) ||
        !right->bounding_box(time0, time1, box_right))
    {
        std::cerr << "No bounding Box in BVHNode constructor.\n";
    }

    Box = surrounding_box(box_left, box_right);
}

inline bool BVHNode::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    if (!Box.hit(r, t_min, t_max))
    {
        return false;
    }

    const bool hit_left = left->hit(r, t_min, t_max, rec);
    const bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

inline bool BVHNode::bounding_box([[maybe_unused]] double t0,
                                   [[maybe_unused]] double t1,
                                   AABB& output_box) const
{
    output_box = Box;

    return true;
}

