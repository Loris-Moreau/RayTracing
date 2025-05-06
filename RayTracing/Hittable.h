#pragma once

#include "AABB.h"
#include "Ray.h"

class Material;

struct HitRecord
{
    Vector3 p;
    Vector3 normal;
    
    std::shared_ptr<Material> mat_ptr;
    
    double t{0.0};
    double u{0.0};
    double v{0.0};
    
    bool front_face{false};

    void set_face_normal(const Ray& r, const Vector3& outward_normal)
    {
        // dot(r.direction(), outward_normal) > 0.0  => Ray is inside the Sphere
        // dot(r.direction(), outward_normal) <= 0.0 => Ray is outside the
        // Sphere
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
 public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(double t0, double t1, AABB& output_box) const = 0;
};

