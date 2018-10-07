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

extern OLYLUMORAYAPI std::unique_ptr<Image>
raycast(
    Scene *inScene,
    const uint32_t inWidth,
    const uint32_t inHeight,
    const uint32_t inSampleCount,
    const uint32_t inMaxRaysCast,
    const EMode inMode,
    const uint32_t inProgressTick,
    std::function<void(int)> inProgressCallback,
    bool *inAbortState);

} // namespace olylumoray

#endif // OLYLUMORAY_RAYCAST_H
