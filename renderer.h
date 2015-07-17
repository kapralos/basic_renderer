#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "RenderTarget.h"
#include "ZBuffer.h"
#include "geometry.h"
#include <vector>
#include "model.h"
#include <memory>

class Renderer
{
public:
    Renderer(std::shared_ptr<RenderTarget> _renderTarget);
    void render(const Vec3f& light, Model& _model);
    void lookat(const Vec3f& eye, const Vec3f& center, const Vec3f& up);
    void projection(float k);
    void viewport(int x, int y, int width, int height);

private:
    void drawTriangleFilled(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, 
        const Vec2i& uv0, const Vec2i& uv1, const Vec2i& uv2, 
        float intensity, Model& model);

    std::shared_ptr<RenderTarget> renderTarget;
    ZBuffer zbuffer;

    Matrix ModelView;
    Matrix Viewport;
    Matrix Projection;
};

#endif
