#include "Hittable.h"

void HitInfo::SetFaceNormal(const Ray& ray, const Vector3& outwardNormal)
{
	//NOTE : The outwardNormal vector should be unit length
	frontFace = Dot(ray.GetDirection(), outwardNormal) < 0;
	normal = frontFace ? outwardNormal : -outwardNormal;
}
