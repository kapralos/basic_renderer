#include "ZBuffer.h"

ZBuffer::ZBuffer() : ImageTarget()
{
}

ZBuffer::ZBuffer(int width, int height) : ImageTarget(width, height, TGAImage::GRAYSCALE)
{
}

ZBuffer::ZBuffer(int w, int h, const std::string& _filename) : ImageTarget(w, h, TGAImage::GRAYSCALE, _filename)
{
}
