#include "tgaimage.h"
#include "renderer.h"

using namespace std;

const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor white = TGAColor(255, 255, 255, 255);

int main(int argc, char const *argv[])
{
    Renderer renderer(TGAImage(101, 101, TGAImage::RGB));
    
    renderer.drawLine(Point(20, 20), Point(40, 80), red);
    renderer.drawLine(Point(60, 30), Point(10, 20), white);
    renderer.drawLine(Point(90, 10), Point(92, 90), white);

    renderer.save("output.tga");

    return 0;
}
