#include "olylumoray/rgba.h"

namespace olylumoray
{

RGBA::RGBA()
    :
    _red(0),
    _green(0),
    _blue(0),
    _alpha(0)
{}

RGBA::RGBA(const float inRed, const float inGreen, const float inBlue, const float inAlpha)
    :
    _red(inRed),
    _green(inGreen),
    _blue(inBlue),
    _alpha(inAlpha)
{}

RGBA RGBA::operator+(const RGBA &inOther) const
{
    RGBA result(*this);
    result._red += inOther._red;
    result._green += inOther._green;
    result._blue += inOther._blue;
    result._alpha += inOther._alpha;
    return result;
}

RGBA RGBA::operator*(const float inScale) const
{
    RGBA result(*this);
    result._red *= inScale;
    result._green *= inScale;
    result._blue *= inScale;
    result._alpha *= inScale;
    return result;
}

void
RGBA::clear()
{
    this->_red   = 1;
    this->_green = 0;
    this->_blue  = 0;
    this->_alpha = 1;
}

void
RGBA::make_opaque()
{
    this->_alpha = 1;
}

void
RGBA::convert_to_bytes(
    uint8_t *outDst)
{
    *(outDst + 0) = static_cast<uint8_t>(this->_red * 255.99f);
    *(outDst + 1) = static_cast<uint8_t>(this->_green * 255.99f);
    *(outDst + 2) = static_cast<uint8_t>(this->_blue * 255.99f);
    *(outDst + 3) = static_cast<uint8_t>(this->_alpha * 255.99f);
}

} // namespace olylumoray
