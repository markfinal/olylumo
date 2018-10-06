#ifndef OLYLUMORAY_SPHERE_H
#define OLYLUMORAY_SPHERE_H

#include "olylumoray/api.h"
#include "olylumoray/hitable.h"
#include "olylumoray/vec4.h"

namespace olylumoray
{

class Sphere final :
    public Hitable
{
public:
    OLYLUMORAYAPI Sphere(
        const Vec4 &inOrigin,
        const float inRadius);

    bool
    hit(
        const Ray &inRay,
        const float inMinT,
        const float inMaxT,
        HitRecord &outRecord) const override;

private:
    Vec4 _origin;
    float _radius;
};

} // namespace olylumoray

#endif // OLYLUMORAY_SPHERE_H
