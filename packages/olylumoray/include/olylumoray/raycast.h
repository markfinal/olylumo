#ifndef OLYLUMORAY_RAYCAST_H
#define OLYLUMORAY_RAYCAST_H

#include "olylumoray/api.h"

#include <memory>

namespace olylumoray
{

class Image;

extern OLYLUMORAYAPI std::unique_ptr<Image>
raycast();

} // namespace olylumoray

#endif // OLYLUMORAY_RAYCAST_H
