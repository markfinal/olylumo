#ifndef OLYLUMORAY_UTILS_H
#define OLYLUMORAY_UTILS_H

#include "olylumoray/vec4.h"

namespace olylumoray
{

extern float
random_between_zero_and_one();

extern float
random_between_minus_one_and_one();

extern Vec4
random_in_unit_sphere();

Vec4
reflect(
    const Vec4 &inV,
    const Vec4 &inN);

} // namespace olylumoray

#endif // OLYLUMORAY_UTILS_H
