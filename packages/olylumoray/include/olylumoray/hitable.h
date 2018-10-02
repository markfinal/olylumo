#ifndef OLYLUMORAY_HITABLE_H
#define OLYLUMORAY_HITABLE_H

namespace olylumoray
{

class Ray;
struct HitRecord;

class Hitable
{
public:
    virtual ~Hitable() = 0;

    virtual bool
    hit(
        const Ray &inRay,
        const float inMinT,
        const float inMaxT,
        HitRecord &outRecord) const = 0;
};

inline Hitable::~Hitable()
{}

} // namespace olylumoray

#endif // OLYLUMORAY_HITABLE_H
