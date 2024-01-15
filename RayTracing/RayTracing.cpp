// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "Color.h"

using namespace std;

int main(int argc, char* argv[])
{
    //Resolution
    int width = 256, height = 256;

    //Image
    cout << "P3\n" << width << ' ' << height << "\n255\n";

    for (int y = 0; y < height; y++)
    {
        clog << "Progress : " << (y * 100 / height) << " %\n" << flush;

        for (int x = 0; x < width; x++)
        {
            Color pixel(double(x) / (width - 1), double(y) / (height - 1), 1 - (double(x) / (width - 1)));

            WriteColor(cout, pixel);
        }
    }

    clog << "It has been done \n";

    return 0;
}
