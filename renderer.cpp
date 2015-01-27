#include "Renderer.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

bool compareByY(const Vec2i& t0, const Vec2i& t1)
{
    return t0[1] < t1[1];
}

Renderer::Renderer(const TGAImage& renderTarget) : image(renderTarget)
{

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

void Renderer::drawTriangleFilled(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAColor color)
{
    // Line sweeping
    vector<Vec2i> v = sortVerticesByY(t0, t1, t2);

    int x0 = v[0][0], y0 = v[0][1];
    int x1 = v[1][0], y1 = v[1][1];
    int x2 = v[2][0], y2 = v[2][1];
    float a = 0, b = 0;
    float total = y2 - y0;
    int xA = 0, xB = 0;
    for (int y = y0; y <= y2; y++)
    {
        xA = (y <= y1) ? interpolateX(v[0], v[1], y) : interpolateX(v[1], v[2], y);
        xB = interpolateX(v[0], v[2], y);

        Vec2i start = { xA, y };
        Vec2i end = { xB, y };
        drawLine(start, end, color);
    }
}

void Renderer::save(const char* filename)
{
    image.flip_vertically();
    image.write_tga_file(filename);
}

// **************************************
// Private

inline int Renderer::interpolateX(const Vec2i& t0, const Vec2i& t1, int currentY)
{
    float a = (currentY - t0[1]) / (t1[1] - t0[1] + 1.0);
    return (1.0 - a) * t0[0] + a * t1[0];
}

inline std::vector<Vec2i> Renderer::sortVerticesByY(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2)
{
    vector<Vec2i> v;
    v.push_back(t0);
    v.push_back(t1);
    v.push_back(t2);
    sort(v.begin(), v.end(), compareByY);
    return v;
}
