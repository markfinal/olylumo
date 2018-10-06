#include "olylumoray/lambertian.h"
#include "olylumoray/hitrecord.h"
#include "olylumoray/ray.h"
#include "olylumoray/utils.h"

namespace olylumoray
{

Lambertian::Lambertian(
    const RGBA &inAlbedo)
    :
    _albedo(inAlbedo)
{}

bool
Lambertian::scatter(
    const Ray &inRay,
    const HitRecord &inRecord,
    RGBA &outAttenuation,
    Ray &outScattered
) const
{
    (void)inRay;
    const auto target = inRecord._pos + inRecord._normal + random_in_unit_sphere();
    outScattered = Ray(inRecord._pos, target - inRecord._pos);
    outAttenuation = this->_albedo;
    return true;
}

} // namespace olylumoray
