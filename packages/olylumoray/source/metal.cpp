#include "olylumoray/metal.h"
#include "olylumoray/hitrecord.h"
#include "olylumoray/ray.h"
#include "olylumoray/utils.h"

#include <cassert>

namespace olylumoray
{

Metal::Metal(
    const RGBA &inAlbedo,
    const float inRoughness)
    :
    _albedo(inAlbedo)
{
    assert(inRoughness >= 0);
    if (inRoughness < 1)
    {
        this->_roughness = inRoughness;
    }
    else
    {
        this->_roughness = 1;
    }
}

bool
Metal::scatter(
    const Ray &inRay,
    const HitRecord &inRecord,
    RGBA &outAttenuation,
    Ray &outScattered
) const
{
    const auto reflected = reflect(inRay.direction().normalise(), inRecord._normal);
    outScattered = Ray(inRecord._pos, reflected + random_in_unit_sphere() * this->_roughness);
    outAttenuation = this->_albedo;
    return outScattered.direction().dot(inRecord._normal) > 0;
}

} // namespace olylumoray
