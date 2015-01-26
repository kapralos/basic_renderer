#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "tgaimage.h"
#include "geometry.h"

class Renderer
{
public:
    Renderer(const TGAImage& renderTarget);
    void drawLine(const Vec2i& start, const Vec2i& end, TGAColor color);
    void save(const char* filename);

private:
    TGAImage image;
};

#endif
