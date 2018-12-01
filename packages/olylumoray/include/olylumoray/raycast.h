#ifndef OLYLUMORAY_RAYCAST_H
#define OLYLUMORAY_RAYCAST_H

#include "olylumoray/api.h"

#include <memory>
#include <cstdint>
#include <functional>

namespace olylumoray
{

class Scene;
class Image;

enum class EMode
{
    Colour = 0,
    WorldSpaceNormals
};

extern OLYLUMORAYAPI void
raycast(
    Scene *inScene,
    const uint32_t inWidth,
    const uint32_t inHeight,
    const uint32_t inSampleCount,
    const uint32_t inMaxRaysCast,
    const uint32_t inTileCount,
    const EMode inMode,
    const uint32_t inProgressTick,
    std::function<void(int)> inProgressCallback,
    std::function<void(uint32_t, uint32_t, std::unique_ptr<Image>)> inTileCompleteCallback,
    std::function<void(void)> inCompleteCallback,
    bool *inAbortState);

} // namespace olylumoray

#endif // OLYLUMORAY_RAYCAST_H
