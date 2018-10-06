#include "olylumoray/metal.h"
#include "olylumoray/hitrecord.h"
#include "olylumoray/ray.h"

namespace olylumoray
{

namespace
{

Vec4
reflect(
    const Vec4 &inV,
    const Vec4 &inN)
{
    return inV - inN * 2 * inV.dot(inN);
}

} // anonymous namespace

Metal::Metal(
    const RGBA &inAlbedo)
    :
    _albedo(inAlbedo)
{}

bool
Metal::scatter(
    const Ray &inRay,
    const HitRecord &inRecord,
    RGBA &outAttenuation,
    Ray &outScattered
) const
{
    const auto reflected = reflect(inRay.direction().normalise(), inRecord._normal);
    outScattered = Ray(inRecord._pos, reflected);
    outAttenuation = this->_albedo;
    return outScattered.direction().dot(inRecord._normal) > 0;
}

} // namespace olylumoray
