#include "olylumoray/quadratic.h"

#include <cmath>

namespace olylumoray
{

Quadratic::Quadratic(
    const float inA,
    const float inB,
    const float inC)
    :
    _a(inA),
    _b(inB),
    _c(inC)
{
    this->calculate_discriminant();
}

float
Quadratic::solution1() const
{
    // (-b - sqrt(disc))/2a
    return (-this->_b - sqrtf(this->_discriminant)) / (2 * this->_a);
}

float
Quadratic::solution2() const
{
    // (-b + sqrt(disc))/2a
    return (-this->_b + sqrtf(this->_discriminant)) / (2 * this->_a);
}

float
Quadratic::discriminant() const
{
    return this->_discriminant;
}

void
Quadratic::calculate_discriminant()
{
    // b^2 - 4 * a * c
    this->_discriminant = this->_b * this->_b - 4 * this->_a * this->_c;
}

} // namespace olylumoray
