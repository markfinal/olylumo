#include "olylumoray/lambertian.h"
#include "olylumoray/hitrecord.h"
#include "olylumoray/ray.h"

#include <random>

namespace
{

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> cube_dist(-1, 1);

} // anonymous namespace

namespace olylumoray
{

namespace
{

Vec4
random_in_unit_sphere()
{
    Vec4 p;
    do
    {
        p = Vec4(cube_dist(gen), cube_dist(gen), cube_dist(gen), 0);
    }
    while (p.squared_length() >= 1.0f);
    return p;
}

} // anonymous namespace

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
