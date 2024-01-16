#pragma once

#include "Materials.h"

class Metal :public Materials
{
public:
	Metal(const Color& baseColor) : albedo(baseColor) {}

	bool Scatter(const Ray& rRayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;

private:
	Color albedo;
};
