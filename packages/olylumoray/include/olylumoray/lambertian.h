#ifndef OLYLUMORAY_LAMBERTIAN_H
#define OLYLUMORAY_LAMBERTIAN_H

#include "olylumoray/api.h"
#include "olylumoray/material.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

class Lambertian final :
    public Material
{
public:
    OLYLUMORAYAPI Lambertian(
        const RGBA &inAlbedo);

    bool
    scatter(
        const Ray &inRay,
        const HitRecord &inRecord,
        RGBA &outAttenuation,
        Ray &outScattered
    ) const override;

private:
    RGBA _albedo;
};

} // namespace olylumoray

#endif // OLYLUMORAY_LAMBERTIAN_H
