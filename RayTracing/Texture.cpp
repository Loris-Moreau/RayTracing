#include "Texture.h"

Color SolidColor::Value(double x, double y, const Position& position) const
{
    return colorValue;
}

Color CheckerTexture::Value(double x, double y, const Position& position) const
{
    int xInteger = static_cast<int>(std::floor(inverseScale * position.x));
    int yInteger = static_cast<int>(std::floor(inverseScale * position.y));
    int zInteger = static_cast<int>(std::floor(inverseScale * position.z));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? even->Value(x, y, position) : odd->Value(x, y, position);
}