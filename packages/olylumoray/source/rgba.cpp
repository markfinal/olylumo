#include "olylumoray/rgba.h"

namespace olylumoray
{

void
RGBA::clear()
{
    this->_red   = 255;
    this->_green = 0;
    this->_blue  = 0;
    this->_alpha = 255;
}

void
RGBA::set(
    const float inRed,
    const float inGreen,
    const float inBlue,
    const float inAlpha)
{
    this->_red   = static_cast<uint8_t>(inRed * 255);
    this->_green = static_cast<uint8_t>(inGreen * 255);
    this->_blue  = static_cast<uint8_t>(inBlue * 255);
    this->_alpha = static_cast<uint8_t>(inAlpha * 255);
}

} // namespace olylumoray
