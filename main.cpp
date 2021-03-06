#include "tgaimage.h"
#include "renderer.h"
#include "model.h"
#include <memory>
#include "ImageTarget.h"
#include "CustomShaders.h"

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
    Vec3f light = { 1, 1, 1 };
    Vec3f eye = { 1, 1, 3 };
    Vec3f center = { 0, 0, 0 };
    Vec3f up = { 0, 1, 0 };

    shared_ptr<ImageTarget> target(new ImageTarget(width, height, "output.tga"));
    shared_ptr<TextureShader> shader(new TextureShader());
    shader->setLight(light);

    Renderer renderer(static_pointer_cast<RenderTarget>(target));
    renderer.lookat(eye, center, up);
    renderer.viewport(0, 0, width, height);
    renderer.projection(-1.f / (eye - center).norm());
    renderer.setModel(std::make_shared<Model>(model));
    renderer.setShader(shader);
    renderer.render();

    return 0;
}
