#include "olylumoray/raycast.h"

namespace olylumoray
{

RGBA::RGBA()
{}

void
RGBA::clear()
{
    this->_red = 255;
    this->_green = 0;
    this->_blue = 0;
    this->_alpha = 255;
}

Image::Image(
    const uint32_t inWidth,
    const uint32_t inHeight)
    :
    _width(inWidth),
    _height(inHeight)
{
    this->_pixels.reset(new RGBA[inWidth * inHeight]);
    this->clear();
}

const RGBA *
Image::pixels() const
{
    return this->_pixels.get();
}

uint32_t
Image::width() const
{
    return this->_width;
}

uint32_t
Image::height() const
{
    return this->_height;
}

void
Image::clear()
{
    auto current_pixel = this->_pixels.get();
    for (auto row = 0u; row < this->_height; ++row)
    {
        for (auto col = 0u; col < this->_width; ++col)
        {
            current_pixel->clear();
            current_pixel++;
        }
    }
}

std::unique_ptr<Image>
raycast()
{
    std::unique_ptr<Image> image(new Image(256, 256));
    return image;
}

} // namespace olylumoray
