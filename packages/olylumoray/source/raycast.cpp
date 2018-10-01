#include "olylumoray/raycast.h"
#include "olylumoray/image.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

std::unique_ptr<Image>
raycast()
{
    const auto width = 640u;
    const auto height = 480u;
    std::unique_ptr<Image> image(new Image(width, height));

    auto current_pixel = image->pixels();
    for (auto row = 0u; row < height; ++row)
    {
        const auto g = static_cast<float>(row) / height;
        for (auto col = 0u; col < width; ++col)
        {
            const auto r = static_cast<float>(col) / width;
            current_pixel->set(r, g, 0.4f, 1);
            current_pixel++;
        }
    }

    return image;
}

} // namespace olylumoray
