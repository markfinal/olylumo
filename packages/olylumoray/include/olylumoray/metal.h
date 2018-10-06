#ifndef OLYLUMORAY_METAL_H
#define OLYLUMORAY_METAL_H

#include "olylumoray/api.h"
#include "olylumoray/material.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

class Metal final :
    public Material
{
public:
    OLYLUMORAYAPI Metal(
        const RGBA &inAlbedo,
        const float inRoughness);

    bool
    scatter(
        const Ray &inRay,
        const HitRecord &inRecord,
        RGBA &outAttenuation,
        Ray &outScattered
    ) const override;

private:
    RGBA  _albedo;
    float _roughness;
};

} // namespace olylumoray

#endif // OLYLUMORAY_METAL_H
