#include "Texture.h"

Color CheckerTexture::Value(const double x, const double y, const Position& position) const
{
    const int xInteger = static_cast<int>(std::floor(inverseScale * position.x));
    const int yInteger = static_cast<int>(std::floor(inverseScale * position.y));
    const int zInteger = static_cast<int>(std::floor(inverseScale * position.z));

    const bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? even->Value(x, y, position) : odd->Value(x, y, position);
}
