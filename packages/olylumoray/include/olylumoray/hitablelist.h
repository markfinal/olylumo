#ifndef OLYLUMORAY_HITABLELIST_H
#define OLYLUMORAY_HITABLELIST_H

#include "olylumoray/api.h"
#include "olylumoray/hitable.h"

#include <list>
#include <memory>

namespace olylumoray
{

class HitableList :
    public Hitable
{
public:
    OLYLUMORAYAPI void
    append(
        Hitable *inEntry);

public:
    OLYLUMORAYAPI bool
    hit(
        const Ray &inRay,
        const float inMinT,
        const float inMaxT,
        HitRecord &outRecord) const override;

private:
    std::list<std::unique_ptr<Hitable>> _entries;
};

} // namespace olylumoray

#endif // OLYLUMORAY_HITABLELIST_H
