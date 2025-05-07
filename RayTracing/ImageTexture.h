#pragma once

#include "Texture.h"

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(pop)
#endif

#include <iostream>

class ImageTexture final : public Texture
{
 public:
    const static int bytes_per_pixel = 3;

    ImageTexture() = default;
    ImageTexture(const char* filename);
    ~ImageTexture();
    
    color value(double u, double v, [[maybe_unused]] const point3& p) const override;

 private:
    unsigned char* data = nullptr;
    
    int width = 0;
    int height = 0;
    
    int bytes_per_scanline = 0;
};
