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
    const int depth = 250;
    Renderer renderer(TGAImage(width, height, TGAImage::RGB));
    
    Vec3f light = { 0, 0, -1 };
    for (int i = 0; i < numFaces; i++)
    {
        vector<int> face = model.face(i);
        Vec3i screen[3];
        Vec3f world[3];
        for (int j = 0; j < 3; j++)
        {
            world[j] = model.vert(face[j]);
            Vec3i s = { (world[j][0] + 1) * width / 2, (world[j][1] + 1) * height / 2, (world[j][2] + 1) * depth / 2 };
            screen[j] = s;
        }

        Vec3f n = cross(world[2] - world[0], world[1] - world[0]);
        Vec3f norm = n.normalize();
        float intensity = norm * light;
        if (intensity > 0)
        {
            renderer.drawTriangleFilled(screen[0], screen[1], screen[2], TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255));
        }
    }

    renderer.save("output.tga");

    return 0;
}
