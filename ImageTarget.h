#ifndef __IMAGETARGET_H__
#define __IMAGETARGET_H__

#include "RenderTarget.h"
#include "tgaimage.h"

class ImageTarget : public RenderTarget
{
public:
    ImageTarget();
    ImageTarget(int width, int height);
    ImageTarget(int w, int h, const std::string& _filename);
    virtual ~ImageTarget();
    virtual Color get(int x, int y) override;
    virtual bool set(int x, int y, const Color &c) override;
    virtual int getWidth() override;
    virtual int getHeight() override;

protected:
    ImageTarget(int w, int h, int bpp, const std::string& _filename = "");

    TGAImage image;
    std::string filename;
};

#endif
