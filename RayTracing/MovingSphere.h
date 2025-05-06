#pragma once

#include <utility>

#include "Hittable.h"
#include "Vector3.h"

class MovingSphere final : public Hittable
{
 public:
    MovingSphere() = default;
    MovingSphere(Vector3 cen0, Vector3 cen1, double t0, double t1, double r, std::shared_ptr<Material> m)
        : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(std::move(m)) {}

    bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;
    bool bounding_box(double t0, double t1, AABB& output_box) const override;

    Vector3 center(double time) const;

    Vector3 center0;
    Vector3 center1;
    
    double time0{0.0};
    double time1{0.0};
    
    double radius{0.0};
    
    std::shared_ptr<Material> mat_ptr;
};

inline bool MovingSphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    const Vector3 oc = r.origin() - center(r.time());
    const double a = r.direction().length_squared();
    const double half_b = dot(oc, r.direction());
    const double c = oc.length_squared() - radius * radius;
    const double discriminant = half_b * half_b - a * c;
    
    if (discriminant > 0)
    {
        const double root = sqrt(discriminant);
        double temp = (-half_b - root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const Vector3 outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }

        temp = (-half_b + root) / a;

        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.at(rec.t);

            const Vector3 outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }

    return false;
}

inline bool MovingSphere::bounding_box(double t0, double t1,
                                        AABB& output_box) const
{
    const AABB box0(center(t0) - Vector3(radius, radius, radius),
                    center(t0) + Vector3(radius, radius, radius));
    
    const AABB box1(center(t1) - Vector3(radius, radius, radius),
                    center(t1) + Vector3(radius, radius, radius));

    output_box = surrounding_box(box0, box1);

    return true;
}

inline Vector3 MovingSphere::center(double time) const
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

