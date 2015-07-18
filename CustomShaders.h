#ifndef __CUSTOMSHADERS_H__
#define __CUSTOMSHADERS_H__

class GouraudShader : public Shader
{
public:
    virtual ~GouraudShader() {}

    virtual Vec4f vertex(int faceIdx, int vertIdx)
    {
        varying_intensity[vertIdx] = std::max(0.f, model->normal(faceIdx, vertIdx) * uniform_light);
        Vec4f gl_Vertex = embed<4>(model->vert(faceIdx, vertIdx));
        varying_triangle[vertIdx] = Viewport * Projection * ModelView * gl_Vertex;
        return varying_triangle[vertIdx];
    }

    virtual bool fragment(const Vec3f& bary, Color& color)
    {
        float intensity = bary * varying_intensity;
        color = Color(255, 255, 255) * intensity;
        return false;
    }

    void setLight(const Vec3f& light)
    {
        uniform_light = light.normalize();
    }

private:
    Vec3f varying_intensity;
    Vec3f uniform_light;
};


class TextureShader : public Shader
{
public:
    virtual ~TextureShader() {}

    virtual Vec4f vertex(int faceIdx, int vertIdx)
    {
        varying_intensity[vertIdx] = std::max(0.f, model->normal(faceIdx, vertIdx) * uniform_light);
        Vec4f gl_Vertex = embed<4>(model->vert(faceIdx, vertIdx));
        varying_triangle[vertIdx] = Viewport * Projection * ModelView * gl_Vertex;
        Vec2i uv = model->uv(faceIdx, vertIdx);
        varying_uv[vertIdx] = { uv[0], uv[1] };
        return varying_triangle[vertIdx];
    }

    virtual bool fragment(const Vec3f& bary, Color& color)
    {
        float intensity = bary * varying_intensity;
        Vec2f uv = varying_uv[0] * bary[0] + varying_uv[1] * bary[1] + varying_uv[2] * bary[2];
        color = model->diffuse({ uv[0], uv[1] }) * intensity;
        return false;
    }

    void setLight(const Vec3f& light)
    {
        uniform_light = light.normalize();
    }

private:
    Vec3f varying_intensity;
    Vec3f uniform_light;
    Vec2f varying_uv[3];
};

#endif
