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

    const int width = 800;
    const int height = 800;
    const int depth = 250;
    Vec3f light = { 0, 0, -1 };

    Renderer renderer(TGAImage(width, height, TGAImage::RGB), model);
    renderer.render(light, depth);
    renderer.save("output.tga");

    return 0;
}
