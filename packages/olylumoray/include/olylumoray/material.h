#ifndef OLYLUMORAY_MATERIAL_H
#define OLYLUMORAY_MATERIAL_H

namespace olylumoray
{

class Ray;
struct HitRecord;
class RGBA;

class Material
{
public:
    virtual bool
    scatter(
        const Ray &inRay,
        const HitRecord &inRecord,
        RGBA &outAttenuation,
        Ray &outScattered
    ) const = 0;
};

} // namespace olylumoray

#endif // OLYLUMORAY_MATERIAL_H
