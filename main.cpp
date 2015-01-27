#include "tgaimage.h"
#include "renderer.h"
#include "model.h"

using namespace std;

const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);
const char* defaultModelFile = "obj/african_head.obj";

int main(int argc, char const *argv[])
{
    const char* modelFile = (argc == 2) ? argv[1] : defaultModelFile;
    Model model(modelFile);
    int numFaces = model.nfaces();

    const int width = 800;
    const int height = 800;
    Renderer renderer(TGAImage(width, height, TGAImage::RGB));
    
    // Vec2i start, end;
    // for (int i = 0; i < numFaces; i++)
    // {
    //     vector<int> vertexFace = model.face(i);
    //     for (int j = 0; j < 3; j++)
    //     {
    //         Vec3f v0 = model.vert(vertexFace[j]);
    //         Vec3f v1 = model.vert(vertexFace[(j + 1) % 3]);
    //         start[0] = (v0[0] + 1.0) * width / 2.0;
    //         start[1] = (v0[1] + 1.0) * height / 2.0;
    //         end[0] = (v1[0] + 1.0) * width / 2.0;
    //         end[1] = (v1[1] + 1.0) * height / 2.0;

    //         renderer.drawLine(start, end, white);
    //     }
    // }
    Vec2i t0 = { 200, 200 };
    Vec2i t1 = { 100, 400 };
    Vec2i t2 = { 150, 600 };
    renderer.drawTriangleFilled(t0, t1, t2, white);
    renderer.drawTriangle(t0, t1, t2, red);

    renderer.save("output.tga");

    return 0;
}
