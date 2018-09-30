#ifndef OLYLUMORAY_RAY_H
#define OLYLUMORAY_RAY_H

#include "olylumoray/vec4.h"

namespace olylumoray
{

class Ray
{
public:
    Ray(
        const Vec4 &inOrigin,
        const Vec4 &inDirection);

private:
    Vec4 _origin;
    Vec4 _direction;
};

} // namespace olylumoray

#endif // OLYLUMORAY_RAY_H
