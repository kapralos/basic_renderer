#include "Renderer.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

static const float depth = 255.f;
Matrix ModelView;
Matrix Viewport;
Matrix Projection;

// ********************
// Helpers

inline float barycentricPart(const Vec2f& t0, const Vec2f& t1, const Vec2f& t2)
{
    return (t0[1] - t2[1]) * (t1[0] - t2[0]) - (t0[0] - t2[0]) * (t1[1] - t2[1]);
}

Vec3f barycentric(const Vec2f& t0, const Vec2f& t1, const Vec2f& t2, const Vec2f& p)
{
    float x = barycentricPart(p, t1, t2) / barycentricPart(t0, t1, t2);
    float y = barycentricPart(p, t2, t0) / barycentricPart(t1, t2, t0);
    float z = barycentricPart(p, t1, t0) / barycentricPart(t2, t1, t0);
    return { x, y, z };
}


// ********************
// Renderer

Renderer::Renderer(std::shared_ptr<RenderTarget> _renderTarget) : renderTarget(move(_renderTarget))
{
    zbuffer = make_shared<ZBuffer>(ZBuffer(renderTarget->getWidth(), renderTarget->getHeight(), "zbuffer.tga"));
}

void Renderer::render()
{
    // std::cerr << ModelView << std::endl;
    //     std::cerr << Projection << std::endl;
    //     std::cerr << Viewport << std::endl;
    //     Matrix z = (Viewport*Projection*ModelView);
    //     std::cerr << z << std::endl;
    shader->setModel(model);
    int numFaces = model->nfaces();
    // Vec3f light_proj = proj<3>(Projection * ModelView * embed<4>(light, 0.f)).normalize();

    for (int i = numFaces-1; i >= 0; i--)
    {
        vector<int> face = model->face(i);
        // Vec3f world[3];
        // Vec3i screen[3];
        // Vec2i text[3];
        Matrix vertices;
        for (int j = 0; j < 3; j++)
        {
            // Vec3f w = model.vert(face[j]);
            // world[j] = proj<3>(Projection * ModelView * embed<4>(w));
            // Vec3f s = proj<3>(Viewport * Projection * ModelView * embed<4>(w));
            // screen[j] = { s[0], s[1], s[2] };
            // text[j] = model.uv(i, j);
            vertices[j] = shader->vertex(i, j);
        }

        drawTriangleFilled(vertices);


        // Vec3f n = cross(world[1] - world[0], world[2] - world[0]);
        // Vec3f norm = n.normalize();
        // float intensity = norm * light_proj;
        // if (intensity > 0)
        // {
        //     drawTriangleFilled(screen[0], screen[1], screen[2], text[0], text[1], text[2], intensity, model);
        // }
    }
}

void Renderer::lookat(const Vec3f& eye, const Vec3f& center, const Vec3f& up)
{
    Vec3f z = (eye - center).normalize();
    Vec3f x = cross(up, z).normalize();
    Vec3f y = cross(z, x).normalize();

    Matrix minv = Matrix::identity();
    Matrix tr = Matrix::identity();
    for (int i = 0; i < 3; i++)
    {
        minv[0][i] = x[i];
        minv[1][i] = y[i];
        minv[2][i] = z[i];
        tr[i][3] = -center[i];
    }

    ModelView = minv * tr;
}

void Renderer::projection(float k)
{
    Projection = Matrix::identity();
    Projection[3][2] = 0;
}

void Renderer::viewport(int x, int y, int width, int height)
{
    Viewport = Matrix::identity();
    Viewport[0][0] = width / 2.f;
    Viewport[1][1] = height / 2.f;
    Viewport[2][2] = depth / 2.f;
    Viewport[0][3] = x + width / 2.f;
    Viewport[1][3] = y + height / 2.f;
    Viewport[2][3] = depth / 2.f;
}

void Renderer::setModel(std::shared_ptr<Model> _model)
{
    model = _model;
}

void Renderer::setShader(std::shared_ptr<Shader> _shader)
{
    shader = _shader;
}

// ********************
// Private

void Renderer::drawTriangleFilled(const Matrix& vertices)
{
    Vec4f t0 = vertices[0], t1 = vertices[1], t2 = vertices[2];
    int minX = min( t0[0], min(t1[0], t2[0]) );
    int maxX = max( t0[0], max(t1[0], t2[0]) );
    int minY = min( t0[1], min(t1[1], t2[1]) );
    int maxY = max( t0[1], max(t1[1], t2[1]) );
    int u = 0, v = 0;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Vec3f bary = barycentric(proj<2>(t0), proj<2>(t1), proj<2>(t2), { x, y });
            Vec3f bary_clip = { bary[0] / t0[3], bary[1] / t1[3], bary[2] / t2[3] };
            bary_clip = bary_clip / (bary_clip[0] + bary_clip[1] + bary_clip[2]);

            unsigned char zb = max( 0, min( 255, int(bary_clip * proj<3>(vertices.col(2))) ) );
            if (bary_clip[0] < 0 || bary_clip[1] < 0 || bary_clip[2] < 0 || zbuffer->get(x, y)[0] > zb) continue;

            Color color;
            bool discard = shader->fragment(bary_clip, color);
            if (!discard)
            {
                renderTarget->set(x, y, color);
                zbuffer->set(x, y, zb);
            }
            

            // u = bary[0] * uv0[0] + bary[1] * uv1[0] + bary[2] * uv2[0];
            // v = bary[0] * uv0[1] + bary[1] * uv1[1] + bary[2] * uv2[1];
            // Color color = model.diffuse({ u, v });
            // renderTarget->set(x, y, Color(color[2] * intensity, color[1] * intensity, color[0] * intensity, 255));
            // zbuffer.set(x, y, zb);
        }
    }
}
