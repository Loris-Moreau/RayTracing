#pragma once

#include <utility>

#include "Hittable.h"

class YZRect final : public Hittable
{
 public:
    YZRect() = default;
    YZRect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<Material> mat)
        : mp(std::move(mat)), y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k) {}

    bool hit(const Ray& r, double t0, double t1, HitRecord& rec) const override;
    bool bounding_box([[maybe_unused]] double t0, [[maybe_unused]] double t1, AABB& output_box) const override
    {
        // The bounding Box must have non-zero width in each dimension, so pad the X dimension a very small amount.
        output_box = AABB(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
        
        return true;
    }

    std::shared_ptr<Material> mp;
    double y0 = 0.0, y1 = 0.0, z0 = 0.0, z1 = 0.0, k = 0.0;
};

inline bool YZRect::hit(const Ray& r, double t0, double t1, HitRecord& rec) const
{
    const double t = (k - r.origin().x()) / r.direction().x();
    if (t < t0 || t > t1)
    {
        return false;
    }
    
    const double y = r.origin().y() + t * r.direction().y();
    const double z = r.origin().z() + t * r.direction().z();
    
    if (y < y0 || y > y1 || z < z0 || z > z1)
    {
        return false;
    }
    
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    
    const Vector3 outward_normal = Vector3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);

    return true;
}
