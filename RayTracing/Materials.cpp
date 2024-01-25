#include "Materials.h"

Color Materials::Emitted(double u, double v, const Position& p) const
{
    return Color(0, 0, 0);
}

bool Lambertian::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
	Vector3 scatterDirection = hitInfo.normal + RandomUnitVector();

	//Catch invalid directions
	if (scatterDirection.NearZero()) scatterDirection = hitInfo.normal;

	scattered = Ray(hitInfo.coordinates, scatterDirection, rayIn.time());
	attenuation = albedo->Value(hitInfo.x, hitInfo.y, hitInfo.coordinates);

	return true;
}

bool Dielectric::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
    attenuation = Color(1.0, 1.0, 1.0);

    double refractionRatio = hitInfo.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;

    Vector3 unitDirection = Unit(rayIn.GetDirection());

    double cosTheta = fmin(Dot(-unitDirection, hitInfo.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = refractionRatio * sinTheta > 1.0;

    Vector3 direction;

    if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble())
    {
        direction = Reflect(unitDirection, hitInfo.normal);
    }
    else
    {
        direction = Refract(unitDirection, hitInfo.normal, refractionRatio);
    }

    scattered = Ray(hitInfo.coordinates, direction, rayIn.time());

    return true;
}

double Dielectric::Reflectance(double cosine, double reflectanceIndex)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - reflectanceIndex) / (1 + reflectanceIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool Metal::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
    Vector3 reflected = Reflect(Unit(rayIn.GetDirection()), hitInfo.normal);
    scattered = Ray(hitInfo.coordinates, reflected + fuzz * RandomUnitVector(), rayIn.time());
    attenuation = albedo;

    return (Dot(scattered.GetDirection(), hitInfo.normal) > 0);
}

bool Isotropic::Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const
{
    scattered = Ray(hitInfo.coordinates, RandomUnitVector(), rayIn.time());
    attenuation = albedo->Value(hitInfo.x, hitInfo.y, hitInfo.coordinates);

    return true;
}
