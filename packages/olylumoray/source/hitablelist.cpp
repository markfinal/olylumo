#include "olylumoray/hitablelist.h"
#include "olylumoray/hitrecord.h"

#include <list>
#include <memory>

namespace olylumoray
{

struct HitableList::Impl
{
    std::list<std::unique_ptr<Hitable>> _entries;
};

HitableList::HitableList()
    :
    _impl(new Impl)
{}

HitableList::~HitableList()
{
    delete this->_impl;
    this->_impl = nullptr;
}

void
HitableList::append(
    Hitable *inEntry)
{
    this->_impl->_entries.emplace_back(inEntry);
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
    for (const auto &hittable : this->_impl->_entries)
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
