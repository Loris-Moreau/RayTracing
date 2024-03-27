#pragma once

#include "Color.h"
//#include "rtw_stb_image.h"
#include "Perlin.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Color Value(double x, double y, const Position& position) const = 0;
};

class SolidColor final : public Texture
{
public:
    SolidColor(const Color& color) : colorValue(color) {}

    SolidColor(const double red, const double green, const double blue) : SolidColor(Color(red, green, blue)) {}

    Color Value(double x, double y, const Position& position) const override { return colorValue; }

private:
    Color colorValue;
};

class CheckerTexture final : public Texture
{
public:
    CheckerTexture(const double _scale, const shared_ptr<Texture>& _even, const shared_ptr<Texture>& _odd)
        : inverseScale(1.0 / _scale), even(_even), odd(_odd) {}

    CheckerTexture(const double _scale, Color color1, Color color2)
        : inverseScale(1.0 / _scale), even(make_shared<SolidColor>(color1)), odd(make_shared<SolidColor>(color2)) {}

    Color Value(double x, double y, const Position& position) const override;

private:
    double inverseScale;

    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

/*
class ImageTexture : public Texture
{
public:
    ImageTexture(const char* filename) : image(filename) {}

    Color Value(double u, double v, const Position& position) const override
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (image.height() <= 0) return Color(0, 1, 1);

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Interval(0, 1).Clamp(u);
        v = 1.0 - Interval(0, 1).Clamp(v);  // Flip V to image coordinates

        int i = static_cast<int>(u * image.width());
        int j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i, j);

        double color_scale = 1.0 / 255.0;
        return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    rtw_image image;
};
*/

class NoiseTexture final : public Texture
{
public:
    NoiseTexture() = default;
    explicit NoiseTexture(const double _scale) : scale(_scale) {}

    Color Value(double U, double V, const Position& position) const override
    {
        const Vector3 scale_ = scale * position;
        return Color(1, 1, 1) * 0.5 * (0.9 + sin(scale_.z + 12 * noise.Turbulence(scale_)));
    }

private:
    Perlin noise;

    double scale;
};
