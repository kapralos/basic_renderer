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
    void render(const Vec3f& light, int depth, float zCamera, Model& _model);

private:
    void drawTriangleFilled(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, 
        const Vec2i& uv0, const Vec2i& uv1, const Vec2i& uv2, 
        float intensity, Model& model);

    std::shared_ptr<RenderTarget> renderTarget;
    ZBuffer zbuffer;
};

#endif
