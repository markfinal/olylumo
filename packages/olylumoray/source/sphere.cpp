#include "olylumoray/sphere.h"
#include "olylumoray/quadratic.h"
#include "olylumoray/ray.h"
#include "olylumoray/hit_record.h"

namespace olylumoray
{

Sphere::Sphere(
    const Vec4 &inOrigin,
    const float inRadius)
    :
    _origin(inOrigin),
    _radius(inRadius)
{}

bool
Sphere::hit(
    const Ray &inRay,
    const float inMinT,
    const float inMaxT,
    HitRecord &outRecord) const
{
    const auto distance = inRay.origin() - this->_origin;
    Quadratic q(
        inRay.direction().dot(inRay.direction()),
        2 * inRay.direction().dot(distance),
        distance.dot(distance) - this->_radius * this->_radius
    );
    if (q.discriminant() > 0)
    {
        auto t = q.solution1();
        if (t > inMinT && t < inMaxT)
        {
            outRecord._t = t;
            outRecord._pos = inRay.pos(t);
            outRecord._normal = (outRecord._pos - this->_origin) / this->_radius;
            return true;
        }
        t = q.solution2();
        if (t > inMinT && t < inMaxT)
        {
            outRecord._t = t;
            outRecord._pos = inRay.pos(t);
            outRecord._normal = (outRecord._pos - this->_origin) / this->_radius;
            return true;
        }
    }
    return false;
}

} // namespace olylumoray
