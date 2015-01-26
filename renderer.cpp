#include "Renderer.h"
#include <cmath>

using namespace std;

Renderer::Renderer(const TGAImage& renderTarget) : image(renderTarget)
{

}

void Renderer::drawLine(const Point& start, const Point& end, TGAColor color)
{
    int x0 = start.x, y0 = start.y;
    int x1 = end.x, y1 = end.y;
    // bool steep = false;
    // if (abs(x1 - x0) < abs(y1 - y0))
    // {
    //         swap(x0, y0);
    //         swap(x1, y1);
    //         steep = true;
    // }

    // if (x1 < x0)
    // {
    //     swap(x0, x1);
    //     swap(y0, y1);
    // }

    // for (int x = x0; x <= x1; x++)
    // {
    //     float t = (x - x0) / float(x1 - x0);
    //     int y = (1.0 - t) * y0 + t * y1;
    //     if (steep)
    //     {
    //         image.set(y, x, color);
    //     }
    //     else
    //     {
    //         image.set(x, y, color);
    //     }
    // }

    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    float derror = std::abs(dy/float(dx));
    float error = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error += derror;

        if (error>.5) {
            y += (y1>y0?1:-1);
            error -= 1.;
        }
    }
}

void Renderer::save(const char* filename)
{
    image.flip_vertically();
    image.write_tga_file(filename);
}
