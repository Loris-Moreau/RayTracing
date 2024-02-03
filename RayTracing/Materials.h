#pragma once

#include "Color.h"
#include "Hittable.h"
#include "Texture.h"
#include "ONB.h"
#include "PDF.h"

class HitInfo;

class ScaterInfo
{
public:
    Color attenuation;
    shared_ptr<PDF> pdf_ptr;
    bool skipPDF;
    Ray skipPDFRay;
};

class Materials
{
public:
    virtual ~Materials() {}

    virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const = 0;

    virtual double ScatteringPDF(const Ray& rayIn, const HitInfo& hitInfo, const Ray& scattered) const;

    virtual Color Emitted(double U, double V, const Position& position) const;
    virtual Color Emitted(const Ray& rayIn, const HitInfo hitInfo, double u, double v, const Position& position) const;
};

class Lambertian : public Materials
{
public:
    Lambertian(const Color& a) : albedo(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo(a) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const override;

    double ScatteringPDF(const Ray& rayIn, const HitInfo& hitInfo, const Ray& scattered) const override;

private:
    shared_ptr<Texture> albedo;
};

class Dielectric : public Materials
{
public:
    Dielectric(double refractionIndex) : indexOfRefraction(refractionIndex) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const override;

    static double Reflectance(double cosine, double reflectanceIndex);

private:
    double indexOfRefraction; //Index of Refraction
};

class Metal :public Materials
{
public:
    Metal(const Color& baseColor, double fuzzines) : albedo(baseColor), fuzz(fuzzines < 1 ? fuzzines : 1) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const override;

private:
    Color albedo;
    double fuzz;
};

class Isotropic :public Materials
{
    //This code assumes that the boundary shape is convex. 
    //So this particular implementation will work for boundaries like boxes or spheres, but will not work with toruses or shapes that contain voids.
public:
    Isotropic(Color color) : albedo(make_shared<SolidColor>(color)) {}
    Isotropic(shared_ptr<Texture> a) : albedo(a) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const override;

    double ScatteringPDF(const Ray& rayIn, const HitInfo& hitInfo, const Ray& scattered) const override;

private:
    shared_ptr<Texture> albedo;
};

class DiffuseLight : public Materials
{
public:
    DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
    DiffuseLight(Color c) : emit(make_shared<SolidColor>(c)) {}

    bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, ScaterInfo& scatterInfo) const override;

    Color Emitted(double U, double V, const Position& position) const override;
    Color Emitted(const Ray& rayIn, const HitInfo hitInfo, double u, double v, const Position& position) const override;

private:
    shared_ptr<Texture> emit;
};
