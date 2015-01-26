#include "tgaimage.h"

const TGAColor red = TGAColor(255, 0, 0, 255);

int main(int argc, char const *argv[])
{
    TGAImage image(100, 100, TGAImage::RGB);
    image.set(51, 40, red);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
