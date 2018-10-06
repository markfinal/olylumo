#ifndef OLYLUMORAY_METAL_H
#define OLYLUMORAY_METAL_H

#include "olylumoray/material.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

class Metal final :
    public Material
{
public:
    Metal(
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

#endif // OLYLUMORAY_METAL_H
