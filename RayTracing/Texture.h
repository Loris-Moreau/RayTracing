#pragma once

#include "Color.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Color value(double x, double y, const Position& position) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(Color color) : colorValue(color) {}

    SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

    Color Value(double x, double y, const Position& position) const override;

private:
    Color colorValue;
};
