#include "olylumoray/ray.h"

namespace olylumoray
{

Ray::Ray(
    const Vec4 &inOrigin,
    const Vec4 &inDirection)
    :
    _origin(inOrigin),
    _direction(inDirection)
{}

} // namespace olylumoray
