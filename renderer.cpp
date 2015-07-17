#include "Renderer.h"
#include <cmath>
#include <algorithm>
#include <iostream>

using namespace std;

// ********************
// Helpers

inline float barycentricPart(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2)
{
    return (t0[1] - t2[1]) * (t1[0] - t2[0]) - (t0[0] - t2[0]) * (t1[1] - t2[1]);
}

Vec3f barycentric(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, const Vec3i& p)
{
    float x = barycentricPart(p, t1, t2) / barycentricPart(t0, t1, t2);
    float y = barycentricPart(p, t2, t0) / barycentricPart(t1, t2, t0);
    float z = barycentricPart(p, t1, t0) / barycentricPart(t2, t1, t0);
    return { x, y, z };
}

Vec3f perspective(const Vec3f& vertex, float zCamera)
{
    Matrix mat;
    mat[0] = { 1., 0., 0., 0. };
    mat[1] = { 0., 1., 0., 0. };
    mat[2] = { 0., 0., 1., 0. };
    mat[3] = { 0., 0., -1/zCamera, 1. };
    Vec4f w = { vertex[0], vertex[1], vertex[2], 1.0 };
    w = mat * w;
    return { w[0]/(1 - w[2]/zCamera), w[1]/(1 - w[2]/zCamera), w[2]/(1 - w[2]/zCamera) };
}


// ********************
// Renderer

Renderer::Renderer(std::shared_ptr<RenderTarget> _renderTarget) : renderTarget(move(_renderTarget))
{
    zbuffer = ZBuffer(renderTarget->getWidth(), renderTarget->getHeight(), "zbuffer.tga");
}

void Renderer::render(const Vec3f& light, int depth, float zCamera, Model& model)
{
    int numFaces = model.nfaces();
    int width = renderTarget->getWidth();
    int height = renderTarget->getHeight();

    for (int i = 0; i < numFaces; i++)
    {
        vector<int> face = model.face(i);
        Vec3i screen[3];
        Vec3f world[3];
        Vec2i text[3];
        for (int j = 0; j < 3; j++)
        {
            world[j] = model.vert(face[j]);
            world[j] = perspective(world[j], zCamera);

            Vec3i s = { (world[j][0] + 1) * width / 2, (world[j][1] + 1) * height / 2, (world[j][2] + 1) * depth / 2 };
            screen[j] = s;
            text[j] = model.uv(i, j);
        }



        Vec3f n = cross(world[2] - world[0], world[1] - world[0]);
        Vec3f norm = n.normalize();
        float intensity = norm * light;
        if (intensity > 0)
        {
            drawTriangleFilled(screen[0], screen[1], screen[2], text[0], text[1], text[2], intensity, model);
        }
    }
}

// ********************
// Private

void Renderer::drawTriangleFilled(const Vec3i& t0, const Vec3i& t1, const Vec3i& t2, 
    const Vec2i& uv0, const Vec2i& uv1, const Vec2i& uv2, 
    float intensity, Model& model)
{
    int minX = min( t0[0], min(t1[0], t2[0]) );
    int maxX = max( t0[0], max(t1[0], t2[0]) );
    int minY = min( t0[1], min(t1[1], t2[1]) );
    int maxY = max( t0[1], max(t1[1], t2[1]) );
    int u = 0, v = 0;

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Vec3f bary = barycentric(t0, t1, t2, { x, y, 0 });
            unsigned char zb = max( 0, min( 255, int(bary[0] * t0[2] + bary[1] * t1[2] + bary[2] * t2[2]) ) );
            if (bary[0] < 0 || bary[1] < 0 || bary[2] < 0 || zbuffer.get(x, y)[0] > zb) continue;

            u = bary[0] * uv0[0] + bary[1] * uv1[0] + bary[2] * uv2[0];
            v = bary[0] * uv0[1] + bary[1] * uv1[1] + bary[2] * uv2[1];
            Color color = model.diffuse({ u, v });
            renderTarget->set(x, y, Color(color[2] * intensity, color[1] * intensity, color[0] * intensity, 255));
            zbuffer.set(x, y, zb);
        }
    }
}
