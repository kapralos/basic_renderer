#include "Renderer.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// ********************
// Helpers

inline float barycentricPart(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2)
{
    return (t0[1] - t2[1]) * (t1[0] - t2[0]) - (t0[0] - t2[0]) * (t1[1] - t2[1]);
}

Vec3f barycentric(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, const Vec3i& p)
{
    float x = barycentricPart(p, t1, t2) / barycentricPart(t0, t1, t2);
    float y = barycentricPart(p, t2, t0) / barycentricPart(t1, t2, t0);
    float z = barycentricPart(p, t1, t0) / barycentricPart(t2, t1, t0);
    return { x, y, z };
}


// ********************
// Renderer

Renderer::Renderer(const TGAImage& renderTarget) : image(renderTarget)
{
    zbuffer = TGAImage(image.get_width(), image.get_height(), TGAImage::GRAYSCALE);
}

void Renderer::drawLine(const Vec2i& start, const Vec2i& end, TGAColor color)
{
    // Bresenham algorithm
    int x0 = start[0], y0 = start[1];
    int x1 = end[0], y1 = end[1];

    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) // swap axises if line is steep
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) // draw line from left to right
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror = std::abs(dy) * 2.0; // dispose of division and floats by coeff. 2*dx
    int error = 0;
    int y = y0;
    int inc = y1 > y0 ? 1 : -1;
    for (int x=x0; x<=x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }

        error += derror;
        if (error > dx) // add or subtract if error is more than half a pixel
        {
            y += inc;
            error -= dx * 2.0;
        }
    }
}

void Renderer::drawTriangle(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAColor color)
{
    drawLine(t0, t1, color);
    drawLine(t1, t2, color);
    drawLine(t2, t0, color);
}

void Renderer::drawTriangleFilled(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, TGAColor color)
{
    int minX = min( t0[0], min(t1[0], t2[0]) );
    int maxX = max( t0[0], max(t1[0], t2[0]) );
    int minY = min( t0[1], min(t1[1], t2[1]) );
    int maxY = max( t0[1], max(t1[1], t2[1]) );

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Vec3f bary = barycentric(t0, t1, t2, { x, y, 0 });
            unsigned char zb = max( 0, min( 255, int(bary[0] * t0[2] + bary[1] * t1[2] + bary[2] * t2[2]) ) );
            if (bary[0] < 0 || bary[1] < 0 || bary[2] < 0 || zbuffer.get(x, y)[0] > zb) continue;
            image.set(x, y, color);
            zbuffer.set(x, y, zb);
        }
    }
}

void Renderer::save(const char* filename)
{
    image.flip_vertically();
    image.write_tga_file(filename);
}
