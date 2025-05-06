#include "ImageTexture.h"

ImageTexture::ImageTexture(const char* filename)
{
    int components_per_pixel = bytes_per_pixel;

    data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

    if (!data)
    {
        std::cerr << "ERROR: Could not load Texture image file '" << filename << "'.\n";
        
        width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
}

ImageTexture::~ImageTexture()
{
    delete data;
}

color ImageTexture::value(double u, double v, [[maybe_unused]] const point3& p) const
{
    // If we have no Texture data, then return solid cyan for debugging.
    if (data == nullptr)
    {
        return color(0, 1, 1);
    }

    // Clamp input Texture coordinates to [0,1] x [1,0]
    u = std::clamp(u, 0.0, 1.0);
    // Flip V to image coordinates
    v = 1.0 - std::clamp(v, 0.0, 1.0);
        
    int i = static_cast<int>(u * width);
    int j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates
    // should be less than 1.0
    if (i >= width)
    {
        i = width - 1;
    }
    if (j >= height)
    {
        j = height - 1;
    }

    const double color_scale = 1.0 / 255.0;
    const unsigned char* pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}
