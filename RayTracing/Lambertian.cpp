#include "Lambertian.h"

#include "Hittable.h"

bool Lambertian::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
	Vector3 scatterDirection = hitInfo.normal + RandomUnitVector();

	//Catch invalid directions
	if (scatterDirection.NearZero()) scatterDirection = hitInfo.normal;

	scattered = Ray(hitInfo.coordinates, scatterDirection);
	attenuation = albedo;
	return true;
}
