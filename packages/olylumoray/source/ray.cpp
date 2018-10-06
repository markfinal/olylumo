#include "olylumoray/ray.h"

#include <cassert>

namespace olylumoray
{

Ray::Ray()
{}

Ray::Ray(
    const Vec4 &inOrigin,
    const Vec4 &inDirection)
    :
    _origin(inOrigin),
    _direction(inDirection)
{
    assert(1.0f == inOrigin.w());
    assert(0.0f == inDirection.w());
}

const Vec4 &
Ray::origin() const
{
    return this->_origin;
}

const Vec4 &
Ray::direction() const
{
    return this->_direction;
}

Vec4
Ray::pos(
    const float inT) const
{
    return this->_origin + this->_direction * inT;
}

} // namespace olylumoray
