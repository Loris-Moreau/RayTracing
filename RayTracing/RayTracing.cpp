// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    //Resolution
    int width = 256, height = 256;
    //Image
    cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Simple gray Color
            cout << "120 120 120" << endl;
        }
    }
    return 0;
}
