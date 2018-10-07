#ifndef OLYLUMORAY_HITABLELIST_H
#define OLYLUMORAY_HITABLELIST_H

#include "olylumoray/hitable.h"

namespace olylumoray
{

class HitableList :
    public Hitable
{
public:
    HitableList();
    ~HitableList() override;

    void
    append(
        Hitable *inEntry);

public:
    bool
    hit(
        const Ray &inRay,
        const float inMinT,
        const float inMaxT,
        HitRecord &outRecord) const override;

private:
    struct Impl;
    Impl *_impl = nullptr; // would use std::unique_ptr if it weren't for C4251 on MSVC
};

} // namespace olylumoray

#endif // OLYLUMORAY_HITABLELIST_H
