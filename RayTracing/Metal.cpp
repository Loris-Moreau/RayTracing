#include "Metal.h"

bool Metal::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
	Vector3 reflected = Reflect(Unit(rayIn.GetDirection()), hitInfo.normal);
	scattered = Ray(hitInfo.coordinates, reflected + fuzz * RandomUnitVector());
	attenuation = albedo;

	return (Dot(scattered.GetDirection(), hitInfo.normal) > 0);
}
