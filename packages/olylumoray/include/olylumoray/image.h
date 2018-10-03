#ifndef OLYLUMORAY_IMAGE_H
#define OLYLUMORAY_IMAGE_H

#include "olylumoray/api.h"

#include <cstdint>
#include <memory>

namespace olylumoray
{

class RGBA;

// First RGBA is at the origin (0,0) which is top-left of the image
class Image
{
public:
    Image(
        const uint32_t inWidth,
        const uint32_t inHeight
    );

    OLYLUMORAYAPI ~Image(); // Note: only exported as it's currently moved from dynamic library to app

    OLYLUMORAYAPI RGBA *
    pixels();

    OLYLUMORAYAPI const RGBA *
    pixels() const;

    OLYLUMORAYAPI uint32_t
    width() const;

    OLYLUMORAYAPI uint32_t
    height() const;

private:
    void
    clear();

private:
    uint32_t _width;
    uint32_t _height;
    std::unique_ptr<RGBA> _pixels;
};

} // namespace olylumoray

#endif // OLYLUMORAY_IMAGE_H
