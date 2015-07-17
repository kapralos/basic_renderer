#include "tgaimage.h"
#include "renderer.h"
#include "model.h"
#include <memory>
#include "ImageTarget.h"

using namespace std;

const Color red = Color(255, 0, 0, 255);
const Color white = Color(255, 255, 255, 255);
const char* defaultModelFile = "obj/african_head.obj";

int main(int argc, char const *argv[])
{
    const char* modelFile = (argc == 2) ? argv[1] : defaultModelFile;
    Model model(modelFile);

    const int width = 800;
    const int height = 800;
    const int depth = 250;
    Vec3f light = { 0, 0, -1 };

    shared_ptr<ImageTarget> target(new ImageTarget(width, height, "output.tga"));

    Renderer renderer(static_pointer_cast<RenderTarget>(target), model);
    renderer.render(light, depth, 2.);

    return 0;
}
