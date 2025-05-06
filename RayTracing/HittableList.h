#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Hittable.h"

class HittableList final : public Hittable
{
 public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object)
    {
        add(std::move(object));
    }

    void clear()
    {
        objects.clear();
    }

    void add(std::shared_ptr<Hittable>&& object)
    {
        objects.emplace_back(object);
    }

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& output_box) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const std::shared_ptr<Hittable>& object : objects)
    {
        if (object->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

inline bool HittableList::bounding_box(double t0, double t1, AABB& output_box) const
{
    if (objects.empty())
    {
        return false;
    }

    const AABB temp_box;
    bool first_box = true;

    for (const std::shared_ptr<Hittable>& object : objects)
    {
        if (!object->bounding_box(t0, t1, output_box))
        {
            return false;
        }

        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

