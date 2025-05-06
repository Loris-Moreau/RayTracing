#pragma once

#include <utility>

#include "Hittable.h"

class Translate final : public Hittable
{
 public:
    Translate(std::shared_ptr<Hittable> p, const Vector3& displacement)
        : ptr(std::move(p)), offset(displacement) {}

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& output_box) const override;

    std::shared_ptr<Hittable> ptr;
    Vector3 offset;
};

inline bool Translate::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    const Ray moved_r{r.origin() - offset, r.direction(), r.time()};
    if (!ptr->hit(moved_r, t_min, t_max, rec))
    {
        return false;
    }

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

inline bool Translate::bounding_box(double t0, double t1, AABB& output_box) const
{
    if (!ptr->bounding_box(t0, t1, output_box))
    {
        return false;
    }

    output_box = AABB{output_box.min() + offset, output_box.max() + offset};

    return true;
}

