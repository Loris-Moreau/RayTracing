#pragma once

#include "Color.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Color Value(double x, double y, const Position& position) const = 0;
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

class CheckerTexture : public Texture 
{
public:
    CheckerTexture(double _scale, shared_ptr<Texture> _even, shared_ptr<Texture> _odd) 
        : inverseScale(1.0 / _scale), even(_even), odd(_odd) {}

    CheckerTexture(double _scale, Color color1, Color color2)
        : inverseScale(1.0 / _scale), even(make_shared<SolidColor>(color1)), odd(make_shared<SolidColor>(color2)) {}

    Color Value(double x, double y, const Position& position) const override;

private:
    double inverseScale;

    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};
