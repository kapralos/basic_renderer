#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "RenderTarget.h"
#include "ZBuffer.h"
#include "geometry.h"
#include <vector>
#include "model.h"
#include <memory>

extern Matrix ModelView;
extern Matrix Viewport;
extern Matrix Projection;


class Shader
{
public:
    virtual ~Shader() {}
    virtual Vec4f vertex(int faceIdx, int vertIdx) = 0;
    virtual bool fragment(const Vec3f& bary, Color& color) = 0;

    void setModel(std::shared_ptr<Model> _model) { model = _model; }

protected:
    std::shared_ptr<Model> model;
};


class Renderer
{
public:
    Renderer(std::shared_ptr<RenderTarget> _renderTarget);
    void render();

    void lookat(const Vec3f& eye, const Vec3f& center, const Vec3f& up);
    void projection(float k);
    void viewport(int x, int y, int width, int height);

    void setModel(std::shared_ptr<Model> _model);
    void setShader(std::shared_ptr<Shader> _shader);

private:
    void drawTriangleFilled(const Matrix& vertices);

    std::shared_ptr<RenderTarget> renderTarget;
    std::shared_ptr<ZBuffer> zbuffer;
    std::shared_ptr<Model> model;
    std::shared_ptr<Shader> shader;
};

#endif
