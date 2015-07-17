#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#include "Color.h"

class RenderTarget
{
public:
    RenderTarget() {}
    RenderTarget(int width, int height) {}
    virtual Color get(int x, int y) = 0;
    virtual bool set(int x, int y, const Color &c) = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
};

#endif
