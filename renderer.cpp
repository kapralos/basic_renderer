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

    // if line is stip, swap axises for accurate calculation
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    // draw from left to right
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    // optimize algorithm to remove deletion
    int dx = x1 - x0;
    int dy = y1 - y0;
    // error should be less than a pixel ( [-0.5, 0.5] ), but I use koefficient  2*dx to dispose of floats
    int derror = std::abs(dy) * 2.0;
    int error = 0;
    int y = y0;
    // if error is more than a half-pixel, add or subtract pixel to Y, depending on Y direction
    int inc = y1 > y0 ? 1 : -1;
    for (int x=x0; x<=x1; x++)
    {
        if (steep) // remember that axises might be swapped
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }

        error += derror;
        if (error > dx)
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
    // line sweeping algorithm: sort vertices by Y and draw A and B bounds simoultaneously
    vector<Vec2i> v = sortVerticesByY(t0, t1, t2);
    Vec2i segment1X = drawSegment(v[0], v[1], v[0], v[2], color); // fill for t0.y to t1.y
    Vec2i startA = { segment1X[0], v[1][1] };
    Vec2i startB = { segment1X[1], v[1][1] };
    drawSegment(startA, v[2], startB, v[2], color); // fill the rest
}

void Renderer::save(const char* filename)
{
    image.flip_vertically();
    image.write_tga_file(filename);
}

Vec2i Renderer::drawSegment(const Vec2i& startA, const Vec2i& endA, const Vec2i& startB, const Vec2i& endB, TGAColor color)
{
    // same optimizations as in Bresenham algorithm
    int errorA = 0, errorB = 0;
    int dxA = endA[0] - startA[0];
    int dyA = endA[1] - startA[1];
    int dxB = endB[0] - startB[0];
    int dyB = endB[1] - startB[1];
    int derrorA = abs(dxA) * 2.0;
    int derrorB = abs(dxB) * 2.0;
    int incA = dxA > 0 ? 1 : -1;
    int incB = dxB > 0 ? 1 : -1;
    int xA = startA[0], xB = startB[0];

    for (int y = startA[1]; y <= endA[1]; y++)
    {
        Vec2i point1 = { xA, y };
        Vec2i point2 = { xB, y };
        drawLine(point1, point2, color);

        errorA += derrorA;
        if (errorA > dyA)
        {
            xA += incA;
            errorA -= dyA * 2.0;
        }

        errorB += derrorB;
        if (errorB > dyB)
        {
            xB += incB;
            errorB -= dyB * 2.0;
        }
    }

    return { xA, xB };
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
