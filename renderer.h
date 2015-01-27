#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "tgaimage.h"
#include "geometry.h"
#include <vector>

class Renderer
{
public:
    Renderer(const TGAImage& renderTarget);
    void drawLine(const Vec2i& start, const Vec2i& end, TGAColor color);
    void drawTriangle(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAColor color);
    void drawTriangleFilled(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2, TGAColor color);
    void save(const char* filename);

private:
    Vec2i drawSegment(const Vec2i& startA, const Vec2i& endA, const Vec2i& startB, const Vec2i& endB, TGAColor color);
    std::vector<Vec2i> sortVerticesByY(const Vec2i& t0, const Vec2i& t1, const Vec2i& t2);
    TGAImage image;
};

#endif
