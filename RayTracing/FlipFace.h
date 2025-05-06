#pragma once

#include <utility>

#include "Hittable.h"

class FlipFace final : public Hittable
{
 public:
    FlipFace(std::shared_ptr<Hittable> p) : ptr(std::move(p)) {}

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override
    {
        if (!ptr->hit(r, t_min, t_max, rec))
        {
            return false;
        }

        rec.front_face = !rec.front_face;
        
        return true;
    }

    bool bounding_box(double t0, double t1, AABB& output_box) const override
    {
        return ptr->bounding_box(t0, t1, output_box);
    }

    std::shared_ptr<Hittable> ptr;
};
