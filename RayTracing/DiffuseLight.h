#pragma once

#include "Materials.h"

class DiffuseLight : public Materials
{
public:
    DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
    DiffuseLight(Color c) : emit(make_shared<SolidColor>(c)) {}

    bool Scatter(const Ray& r_in, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;

    Color Emitted(double u, double v, const Position& position) const override;

private:
    shared_ptr<Texture> emit;
};
