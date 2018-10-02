#ifndef OLYLUMORAY_RGBA_H
#define OLYLUMORAY_RGBA_H

#include "olylumoray/api.h"

#include <cstdint>

namespace olylumoray
{

class Vec4;

class RGBA
{
public:
    RGBA();
    RGBA(const float inRed, const float inGreen, const float inBlue, const float inAlpha);
    RGBA(const Vec4 &inOther);

    RGBA operator+(const RGBA &inOther) const;
    RGBA operator*(const float inScale) const;

    void
    clear();

    void
    make_opaque();

    OLYLUMORAYAPI void
    convert_to_bytes(
        uint8_t *outDst);

private:
    float _red;
    float _green;
    float _blue;
    float _alpha;
};

} // namespace olylumoray

#endif // OLYLUMORAY_RGBA_H
