#include "olylumoray/rgba.h"
#include "olylumoray/vec4.h"

#include <cassert>
#include <cmath>

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

RGBA::RGBA(const Vec4 &inOther)
    :
    _red(inOther.x()),
    _green(inOther.y()),
    _blue(inOther.z()),
    _alpha(inOther.w())
{}

RGBA RGBA::operator+(const RGBA &inOther) const
{
    RGBA result(*this);
    result += inOther;
    return result;
}

RGBA &RGBA::operator+=(const RGBA &inOther)
{
    this->_red += inOther._red;
    this->_green += inOther._green;
    this->_blue += inOther._blue;
    this->_alpha += inOther._alpha;
    return *this;
}

RGBA RGBA::operator*(const float inScale) const
{
    RGBA result(*this);
    result *= inScale;
    return result;
}

RGBA &RGBA::operator*=(const float inScale)
{
    this->_red *= inScale;
    this->_green *= inScale;
    this->_blue *= inScale;
    this->_alpha *= inScale;
    return *this;
}

RGBA RGBA::operator/(const float inScale) const
{
    assert(0 != inScale);
    const auto reciprocal = 1.0f / inScale;
    RGBA result(*this);
    result *= reciprocal;
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

RGBA
RGBA::gamma_correct() const
{
    const RGBA result(
        sqrtf(this->_red),
        sqrtf(this->_green),
        sqrtf(this->_blue),
        sqrtf(this->_alpha)
    );
    return result;
}

} // namespace olylumoray
