#ifndef OLYLUMORAY_HITRECORD_H
#define OLYLUMORAY_HITRECORD_H

#include "olylumoray/vec4.h"

namespace olylumoray
{

struct HitRecord
{
    float _t;
    Vec4  _pos;
    Vec4  _normal;
};

} // namespace olylumoray

#endif // OLYLUMORAY_HITRECORD_H
