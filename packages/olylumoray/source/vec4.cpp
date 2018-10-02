#include "olylumoray/vec4.h"

#include <cmath>
#include <cassert>

namespace olylumoray
{

Vec4::Vec4()
    :
    _x(0),
    _y(0),
    _z(0),
    _w(1)
{}

Vec4::Vec4(const float inX, const float inY, const float inZ, const float inW)
    :
    _x(inX),
    _y(inY),
    _z(inZ),
    _w(inW)
{}

float Vec4::x() const
{
    return this->_x;
}

float Vec4::y() const
{
    return this->_y;
}

float Vec4::z() const
{
    return this->_z;
}

float Vec4::w() const
{
    return this->_w;
}

Vec4
Vec4::operator+(
    const Vec4 &inOther) const
{
    Vec4 result(*this);
    result._x += inOther._x;
    result._y += inOther._y;
    result._z += inOther._z;
    result._w += inOther._w;
    return result;
}

Vec4
Vec4::operator+(const float inOther) const
{
    Vec4 result(*this);
    result._x += inOther;
    result._y += inOther;
    result._z += inOther;
    result._w += inOther;
    return result;
}

Vec4
Vec4::operator-(const Vec4 &inOther) const
{
    Vec4 result(*this);
    result._x -= inOther._x;
    result._y -= inOther._y;
    result._z -= inOther._z;
    result._w -= inOther._w;
    return result;
}

Vec4 Vec4::operator*(
    const float inScale) const
{
    Vec4 result(*this);
    result *= inScale;
    return result;
}

Vec4 &Vec4::operator*=(const float inScale)
{
    this->_x *= inScale;
    this->_y *= inScale;
    this->_z *= inScale;
    this->_w *= inScale;
    return *this;
}

Vec4 Vec4::operator/(const float inScale) const
{
    assert(inScale != 0);
    const auto reciprocal = 1.0f / inScale;
    return *this * reciprocal;
}

Vec4
Vec4::normalise() const
{
    Vec4 result(*this);
    const auto length = sqrtf(this->_x * this->_x + this->_y * this->_y + this->_z * this->_z + this->_w * this->_w);
    const auto reciprocal = 1.0f / length;
    result *= reciprocal;
    return result;
}

float
Vec4::dot(const Vec4 &inOther) const
{
    return this->_x * inOther._x + this->_y * inOther._y + this->_z * inOther._z + this->_w * inOther._w;
}

} // namespace olylumoray
