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
    int x0 = start[0], y0 = start[1];
    int x1 = end[0], y1 = end[1];

    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = std::abs(dy) * 2.0;
    float error = 0;
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
    vector<Vec2i> v = sortVerticesByY(t0, t1, t2);

    int x0 = v[0][0], y0 = v[0][1];
    int x1 = v[1][0], y1 = v[1][1];
    int x2 = v[2][0], y2 = v[2][1];
    float a = 0, b = 0;
    float total = y2 - y0;
    int xA = 0, xB = 0;
    for (int y = y0; y <= y2; y++)
    {
        if (y <= y1)
        {
            a = (y - y0) / (y1 - y0 + 1.0);
            xA = (1.0 - a) * x0 + a * x1;
        }
        else
        {
            a = (y - y1) / (y2 - y1 + 1.0);
            xA = (1.0 - a) * x1 + a * x2;
        }
        b = (y - y0) / total;
        xB = (1.0 - b) * x0 + b * x2;

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

inline std::vector<Vec2i> Renderer::sortVerticesByY(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2)
{
    vector<Vec2i> v;
    v.push_back(t0);
    v.push_back(t1);
    v.push_back(t2);
    sort(v.begin(), v.end(), compareByY);
    return v;
}
