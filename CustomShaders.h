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

#endif
