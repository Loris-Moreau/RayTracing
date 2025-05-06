#pragma once

#include "FlipFace.h"
#include "Hittable.h"
#include "HittableList.h"
#include "XYRect.h"
#include "XZRect.h"
#include "YZRect.h"

class Box final : public Hittable
{
 public:
    Box() = default;
    Box(const point3& p0, const point3& p1, const std::shared_ptr<Material>& ptr);
    
    bool hit(const Ray& r, double t0, double t1, HitRecord& rec) const override;
    
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1, AABB& output_box) const override
    {
        output_box = AABB(box_min, box_max);
        return true;
    }
    
    point3 box_min;
    point3 box_max;
    
    HittableList sides;
};

inline Box::Box(const point3& p0, const point3& p1, const std::shared_ptr<Material>& ptr)
    : box_min(p0), box_max(p1)
{
    sides.add(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(std::make_shared<FlipFace>(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr)));

    sides.add(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(std::make_shared<FlipFace>(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr)));

    sides.add(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(std::make_shared<FlipFace>(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr)));
}

inline bool Box::hit(const Ray& r, double t0, double t1, HitRecord& rec) const
{
    return sides.hit(r, t0, t1, rec);
}

