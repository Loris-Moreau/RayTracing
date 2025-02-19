#include "Quadrilaterals.h"

#include <immintrin.h>

Quadrilaterals::Quadrilaterals(const Position& _Q, const Vector3& _u, const Vector3& _v, shared_ptr<Materials> _mat)
    : Q(_Q), u(_u), v(_v), mat(_mat)
{
    Vector3 n = Cross(u, v);
    normal = Unit(n);
    D = Dot(normal, Q);
    w = n / Dot(n, n);

    Quadrilaterals::SetBoundingBox();
}

void Quadrilaterals::SetBoundingBox()
{
    bBox = AABB(Q, Q + u + v).Pad();
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
    double alpha = Dot(w, Cross(planar_hitpt_vector, v));
    double beta = Dot(w, Cross(u, planar_hitpt_vector));

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

// Hit SIMD Optimization Test
/*
bool Quadrilaterals::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
    __m256d normal_vec = _mm256_set_pd(normal.x, normal.y, normal.z, 0.0);
    __m256d ray_dir = _mm256_set_pd(ray.GetDirection().x, ray.GetDirection().y, ray.GetDirection().z, 0.0);
    
    // Dot product of normal and ray direction
    __m256d denom_vec = _mm256_mul_pd(normal_vec, ray_dir);
    double denom = ((double*)&denom_vec)[0] + ((double*)&denom_vec)[1] + ((double*)&denom_vec)[2];
    
    if (fabs(denom) < 1e-8)
    {
        return false;
    }
    
    __m256d ray_origin = _mm256_set_pd(ray.GetOrigin().x, ray.GetOrigin().y, ray.GetOrigin().z, 0.0);
    __m256d dot_origin = _mm256_mul_pd(normal_vec, ray_origin);
    double dot_o = ((double*)&dot_origin)[0] + ((double*)&dot_origin)[1] + ((double*)&dot_origin)[2];
    double t = (D - dot_o) / denom;
    
    if (!rayTime.Contains(t))
    {
        return false;
}
    
    Position intersection = ray.At(t);
    Vector3 planar_hitpt_vector = intersection - Q;
    
    __m256d planar_vec = _mm256_set_pd(planar_hitpt_vector.x, planar_hitpt_vector.y, planar_hitpt_vector.z, 0.0);
    __m256d v_vec = _mm256_set_pd(v.x, v.y, v.z, 0.0);
    __m256d u_vec = _mm256_set_pd(u.x, u.y, u.z, 0.0);
    
    __m256d cross_v = _mm256_mul_pd(planar_vec, v_vec);
    __m256d cross_u = _mm256_mul_pd(u_vec, planar_vec);
    
    double alpha = ((double*)&cross_v)[0] + ((double*)&cross_v)[1] + ((double*)&cross_v)[2];
    double beta = ((double*)&cross_u)[0] + ((double*)&cross_u)[1] + ((double*)&cross_u)[2];
    
    if (!isInterior(alpha, beta, hitInfo))
    {
        return false;
    }
    
    hitInfo.time = t;
    hitInfo.coordinates = intersection;
    hitInfo.mat = mat;
    hitInfo.SetFaceNormal(ray, normal);
    
    return true;
}
*/
// end of Hit SIMD Optimization Test


bool Quadrilaterals::isInterior(double a, double b, HitInfo& hitInfo) const
{
    // Given the hit point in plane coordinates, return false if it is outside the primitive, 
    //  otherwise set the hit record UV coordinates and return true.

    if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
    {
        return false;
    }

    hitInfo.x = a;
    hitInfo.y = b;

    return true;
}
