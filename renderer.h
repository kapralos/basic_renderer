#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "tgaimage.h"

struct Point
{
    Point(int _x, int _y) : x(_x), y(_y) {}
    int x;
    int y;
};

class Renderer
{
public:
    Renderer(const TGAImage& renderTarget);
    void drawLine(const Point& start, const Point& end, TGAColor color);
    void save(const char* filename);

private:
    TGAImage image;
};

#endif
