#include "olylumoray/hitablelist.h"
#include "olylumoray/hitrecord.h"

namespace olylumoray
{

void
HitableList::append(
    Hitable *inEntry)
{
    this->_entries.emplace_back(inEntry);
}

bool
HitableList::hit(
    const Ray &inRay,
    const float inMinT,
    const float inMaxT,
    HitRecord &outRecord) const
{
    auto hit_anything = false;
    float closest = inMaxT;
    for (const auto &hittable : this->_entries)
    {
        if (hittable->hit(inRay, inMinT, closest, outRecord))
        {
            hit_anything = true;
            closest = outRecord._t;
        }
    }
    return hit_anything;
}

} // namespace olylumoray
