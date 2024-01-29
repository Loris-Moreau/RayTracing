#include "Materials.h"

Color Materials::Emitted(double U, double V, const Position& position) const
{
    return Color(0, 0, 0);
}

bool Materials::Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const
{
    return false;
}

double Materials::ScatteringPDF(const Ray& rayIn, const HitInfo& hitInfo, const Ray& scattered) const
{
    return 0;
}

Color Materials::Emitted(const Ray& rayIn, const HitInfo hitInfo, double u, double v, const Position& position) const
{
    return Color(0, 0, 0);
}

bool Lambertian::Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const
{
    scatterInfo.attenuation = albedo->Value(hitInfo.x, hitInfo.y, hitInfo.coordinates);
    scatterInfo.pdf_ptr = make_shared<CosinePDF>(hitInfo.normal);
    scatterInfo.skipPDF = false;

    return true;
}

double Lambertian::ScatteringPDF(const Ray& rayIn, const HitInfo& hitInfo, const Ray& scattered) const
{
    double cosine = Dot(hitInfo.normal, Unit(scattered.GetDirection()));

    return cosine < 0 ? 0 : cosine / pi;
}

bool Dielectric::Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const
{
    scatterInfo.attenuation = Color(1.0, 1.0, 1.0);

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

    Ray scattered = Ray(hitInfo.coordinates, direction, rayIn.time());

    return true;
}

double Dielectric::Reflectance(double cosine, double reflectanceIndex)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - reflectanceIndex) / (1 + reflectanceIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool Metal::Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const
{
    Vector3 reflected = Reflect(Unit(rayIn.GetDirection()), hitInfo.normal);
    Ray scattered = Ray(hitInfo.coordinates, reflected + fuzz * RandomUnitVector(), rayIn.time());
    scatterInfo.attenuation = albedo;

    return (Dot(scattered.GetDirection(), hitInfo.normal) > 0);
}

bool Isotropic::Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const
{
    scatterInfo.attenuation = albedo->Value(hitInfo.x, hitInfo.y, hitInfo.coordinates);
    scatterInfo.pdf_ptr = make_shared<SpherePDF>();
    scatterInfo.skipPDF = false;

    return true;
}

double Isotropic::ScatteringPDF(const Ray& rayIn, const HitInfo& hitInfo, const Ray& scattered) const
{
    return 1 / (4 * pi);
}

bool DiffuseLight::Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const
{
    return false;
}

Color DiffuseLight::Emitted(double U, double V, const Position& position) const
{
    return emit->Value(U, V, position);
}

Color DiffuseLight::Emitted(const Ray& rayIn, const HitInfo hitInfo, double U, double V, const Position& position) const
{
    if (!hitInfo.frontFace)
    {
        return Color(0, 0, 0);
    }
    return emit->Value(U, V, position);
}
