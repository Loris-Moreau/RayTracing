#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
#pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "Externals/stb_image.h"

#include <cstdlib>
#include <stdlib.h>
#include <iostream>

class rtw_image 
{
public:
    rtw_image() : data(nullptr) {}

    rtw_image(const char* imageFilename) 
    {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is
        // defined, looks only in that directory for the image file. If the image was not found,
        // searches for the specified image file first from the current directory, then in the
        // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
        // parent, on so on, for six levels up. If the image was not loaded successfully,
        // width() and height() will return 0.

        std::string filename = std::string(imageFilename);
        auto imagedir = getenv("RTW_IMAGES");

        // Hunt for the image file in some likely locations.
        if (imagedir && Load(std::string(imagedir) + "/" + imageFilename)) return;
        if (Load(filename)) return;
        if (Load("images/" + filename)) return;
        if (Load("../images/" + filename)) return;
        if (Load("../../images/" + filename)) return;
        if (Load("../../../images/" + filename)) return;
        if (Load("../../../../images/" + filename)) return;
        if (Load("../../../../../images/" + filename)) return;
        if (Load("../../../../../../images/" + filename)) return;

        std::cerr << "ERROR : Could not load image file '" << imageFilename << "'.\n";
    }

    ~rtw_image() { STBI_FREE(data); }

    bool Load(const std::string filename) 
    {
        // Loads image data from the given file name. Returns true if the load succeeded.
        auto n = bytes_per_pixel; // Dummy out parameter: original components per pixel
        data = stbi_load(filename.c_str(), &imageWidth, &imageHeight, &n, bytes_per_pixel);
        bytes_per_scanline = imageWidth * bytes_per_pixel;

        return data != nullptr;
    }

    int width()  const { return (data == nullptr) ? 0 : imageWidth; }
    int height() const { return (data == nullptr) ? 0 : imageHeight; }

    const unsigned char* PixelData(int x, int y) const 
    {
        // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
        static unsigned char magenta[] = { 255, 0, 255 };
        if (data == nullptr) return magenta;

        x = Clamp(x, 0, imageWidth);
        y = Clamp(y, 0, imageHeight);

        return data + y * bytes_per_scanline + x * bytes_per_pixel;
    }

private:
    const int bytes_per_pixel = 3;
    unsigned char* data;
    int imageWidth, imageHeight;
    int bytes_per_scanline;

    static int Clamp(int x, int low, int high) 
    {
        // Return the value clamped to the range [low, high).
        if (x < low) return low;

        if (x < high) return x;

        return high - 1;
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif