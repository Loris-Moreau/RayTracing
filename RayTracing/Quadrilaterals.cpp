#include "Quadrilaterals.h"

#include <immintrin.h>

#include <utility>

Quadrilaterals::Quadrilaterals(const Position& _Q, const Vector3& _u, const Vector3& _v, shared_ptr<Materials> _mat)
    : Q(_Q), u(_u), v(_v), mat(std::move(_mat))
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

/*bool Quadrilaterals::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
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
}*/

// Hit SIMD Optimization Test
bool Quadrilaterals::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
    // 1. Pack normal, ray direction, ray origin into AVX registers: (x,y,z,0)
    __m256d normalVec = _mm256_set_pd(0.0, normal.z, normal.y, normal.x);
    __m256d dirVec    = _mm256_set_pd(0.0, ray.GetDirection().z, ray.GetDirection().y, ray.GetDirection().x);
    __m256d origVec   = _mm256_set_pd(0.0, ray.GetOrigin().z,    ray.GetOrigin().y,    ray.GetOrigin().x);

    // 2. Compute denom = Dot(normal, direction)
    double denom = dotAVX(normalVec, dirVec);
    if (fabs(denom) < 1e-8)
    {
        return false;  // Ray is parallel to plane
    }

    // 3. Compute t = (D - Dot(normal, origin)) / denom
    double dotNO = dotAVX(normalVec, origVec);
    double t = (D - dotNO) / denom;
    if (!rayTime.Contains(t))
    {
        return false;  // Intersection is outside valid t-range
    }

    // 4. Get intersection point & planar vector
    Position intersection = ray.At(t);
    Vector3 planarHitVec = intersection - Q;

    // 5. Convert planarHitVec, u, v, w to AVX
    __m256d planarVec = _mm256_set_pd(0.0, planarHitVec.z, planarHitVec.y, planarHitVec.x);
    __m256d vVec      = _mm256_set_pd(0.0, v.z,            v.y,            v.x);
    __m256d uVec      = _mm256_set_pd(0.0, u.z,            u.y,            u.x);
    __m256d wVec      = _mm256_set_pd(0.0, w.z,            w.y,            w.x);
    // NOTE: 'w' was set in the constructor as w = n / Dot(n, n) (where n=Cross(u,v))

    // 6. alpha = Dot(w, Cross(planarHitVec, v))
    __m256d crossPV = crossAVX(planarVec, vVec);
    double alpha = dotAVX(wVec, crossPV);

    // 7. beta = Dot(w, Cross(u, planarHitVec))
    __m256d crossUP = crossAVX(uVec, planarVec);
    double beta = dotAVX(wVec, crossUP);

    // 8. Check if inside [0,1] range
    if (!isInterior(alpha, beta, hitInfo))
    {
        return false;
    }

    // 9. Fill hit info
    hitInfo.time        = t;
    hitInfo.coordinates = intersection;
    hitInfo.mat         = mat;
    hitInfo.SetFaceNormal(ray, normal);

    return true;
}
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
