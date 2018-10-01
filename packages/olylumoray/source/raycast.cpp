#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

std::unique_ptr<Image>
raycast()
{
    std::unique_ptr<Image> image(new Image(640, 480));

    auto current_pixel = image->pixels();
    for (auto row = 0u; row < image->height(); ++row)
    {
        const auto g = static_cast<float>(row) / image->height();
        for (auto col = 0u; col < image->width(); ++col)
        {
            const auto r = static_cast<float>(col) / image->width();
            current_pixel->set(r, g, 0.4f, 1);
            current_pixel++;
        }
    }

    return image;
}

} // namespace olylumoray
