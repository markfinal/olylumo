#ifndef OLYLUMORAY_HIT_RECORD_H
#define OLYLUMORAY_HIT_RECORD_H

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

#endif // OLYLUMORAY_HIT_RECORD_H
