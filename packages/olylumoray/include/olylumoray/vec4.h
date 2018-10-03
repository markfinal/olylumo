#ifndef OLYLUMORAY_VEC4_H
#define OLYLUMORAY_VEC4_H

namespace olylumoray
{

class Vec4
{
public:
    Vec4();
    Vec4(const float inX, const float inY, const float inZ, const float inW);

    float x() const;
    float y() const;
    float z() const;
    float w() const;

    Vec4 operator+(const Vec4 &inOther) const;
    Vec4 operator+(const float inOther) const;
    Vec4 operator-(const Vec4 &inOther) const;
    Vec4 operator*(const float inScale) const;
    Vec4 &operator*=(const float inScale);
    Vec4 operator/(const float inScale) const;

    Vec4 normalise() const;
    float dot(const Vec4 &inOther) const;
    float squared_length() const;

private:
    float _x;
    float _y;
    float _z;
    float _w;
};

} // namespace olylumoray

#endif // OLYLUMORAY_VEC4_H
