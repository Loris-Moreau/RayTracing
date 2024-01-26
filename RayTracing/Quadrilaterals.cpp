#include "Quadrilaterals.h"

Quadrilaterals::Quadrilaterals(const Position& _Q, const Vector3& _u, const Vector3& _v, shared_ptr<Materials> _mat)
    : Q(_Q), U(_u), V(_v), mat(_mat)
{
    Vector3 n = Cross(U, V);
    normal = Unit(n);
    D = Dot(normal, Q);
    W = n / Dot(n, n);

    area = n.Length();

    SetBoundingBox();
}

void Quadrilaterals::SetBoundingBox()
{
    bBox = AABB(Q, Q + U + V).Pad();
}

bool Quadrilaterals::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
    double denom = Dot(normal, ray.GetDirection());

    //No hit if the ray is parallel to the plane.
    if (fabs(denom) < 1e-8)
    {
        return false;
    }

    //Return false if the hit point parameter t is outside the ray interval.
    double t = (D - Dot(normal, ray.GetOrigin())) / denom;
    if (!rayTime.Contains(t))
    {
        return false;
    }

    // Determine the hit point lies within the planar shape using its plane coordinates.
    Position intersection = ray.At(t);
    Vector3 planar_hitpt_vector = intersection - Q;
    double alpha = Dot(W, Cross(planar_hitpt_vector, V));
    double beta = Dot(W, Cross(U, planar_hitpt_vector));

    if (!isInterior(alpha, beta, hitInfo))
    {
        return false;
    }

    // Ray hits the 2D shape; set the rest of the hit record and return true.
    hitInfo.time = t;
    hitInfo.coordinates = intersection;
    hitInfo.mat = mat;
    hitInfo.SetFaceNormal(ray, normal);

    return true;
}

bool Quadrilaterals::isInterior(double a, double b, HitInfo& hitInfo) const
{
    // Given the hit point in plane coordinates, return false if it is outside the primitive, 
    //otherwise set the hit record UV coordinates and return true.

    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
    {
        return false;
    }

    hitInfo.x = a;
    hitInfo.y = b;

    return true;
}
