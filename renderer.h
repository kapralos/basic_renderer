#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "tgaimage.h"
#include "geometry.h"
#include <vector>
#include "model.h"

class Renderer
{
public:
    Renderer(const TGAImage& renderTarget, const Model& _model);
    void render(const Vec3f& light, int depth);
    void save(const char* filename);

private:
    void drawTriangleFilled(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, const Vec2i& uv0, const Vec2i& uv1, const Vec2i& uv2, float intensity);

    TGAImage image;
    TGAImage zbuffer;
    Model model;
};

#endif
