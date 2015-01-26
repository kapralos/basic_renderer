#include "Renderer.h"
#include <cmath>

using namespace std;

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

void Renderer::save(const char* filename)
{
    image.flip_vertically();
    image.write_tga_file(filename);
}
