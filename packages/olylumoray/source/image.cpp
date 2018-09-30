#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

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

Image::~Image() = default;

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
        const auto g = static_cast<float>(row) / this->_height;
        for (auto col = 0u; col < this->_width; ++col)
        {
            const auto r = static_cast<float>(col) / this->_width;
            current_pixel->set(r, g, 0.2f, 1);
            current_pixel++;
        }
    }
}

} // namespace olylumoray
