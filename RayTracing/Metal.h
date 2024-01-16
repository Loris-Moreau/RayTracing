#pragma once

#include "Materials.h"

class Metal :public Materials
{
public:
	Metal(const Color& baseColor, double fuzzines) : albedo(baseColor), fuzz(fuzzines < 1 ? fuzzines : 1) {}

	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;

private:
	Color albedo;
	double fuzz;
};
