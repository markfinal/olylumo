#include "olylumoray/raycast.h"
#include "olylumoray/image.h"

namespace olylumoray
{

std::unique_ptr<Image>
raycast()
{
    std::unique_ptr<Image> image(new Image(640, 480));
    return image;
}

} // namespace olylumoray
