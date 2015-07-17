#include "ImageTarget.h"

ImageTarget::ImageTarget() : RenderTarget()
{
    image = TGAImage();
}

ImageTarget::ImageTarget(int width, int height) : ImageTarget(width, height, TGAImage::RGB)
{
}

ImageTarget::ImageTarget(int width, int height, int bpp, const std::string& _filename) : RenderTarget(width, height)
{
    image = TGAImage(width, height, bpp);
    filename = _filename;
}

ImageTarget::ImageTarget(int w, int h, const std::string& _filename) : ImageTarget(w, h, TGAImage::RGB, _filename)
{
}

ImageTarget::~ImageTarget()
{
    if (filename.length() > 0)
    {
        image.flip_vertically();
        image.write_tga_file(filename.c_str());
    }
}

Color ImageTarget::get(int x, int y)
{
    return image.get(x, y);
}

bool ImageTarget::set(int x, int y, const Color &c)
{
    return image.set(x, y, c);
}

int ImageTarget::getWidth()
{
    return image.get_width();
}

int ImageTarget::getHeight()
{
    return image.get_height();
}
