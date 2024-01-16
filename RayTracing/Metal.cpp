#include "Metal.h"

#include "Hittable.h"

bool Metal::Scatter(const Ray& rRayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
	Vector3 reflected = Reflect(Unit(rRayIn.GetDirection()), hitInfo.normal);
	scattered = Ray(hitInfo.coordinates, reflected);
	attenuation = albedo;
	return true;
}
