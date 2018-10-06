#ifndef OLYLUMORAY_RAYCAST_H
#define OLYLUMORAY_RAYCAST_H

#include "olylumoray/api.h"

#include <memory>
#include <cstdint>

namespace olylumoray
{

class Image;

extern OLYLUMORAYAPI std::unique_ptr<Image>
raycast(
    const uint32_t inWidth,
    const uint32_t inHeight);

} // namespace olylumoray

#endif // OLYLUMORAY_RAYCAST_H
