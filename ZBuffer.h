#ifndef __ZBUFFER_H__
#define __ZBUFFER_H__

#include "ImageTarget.h"

class ZBuffer : public ImageTarget
{
public:
    ZBuffer();
    ZBuffer(int width, int height);
    ZBuffer(int w, int h, const std::string& _filename);
};

#endif
