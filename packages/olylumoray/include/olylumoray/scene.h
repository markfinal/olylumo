#ifndef OLYLUMORAY_SCENE_H
#define OLYLUMORAY_SCENE_H

#include "olylumoray/api.h"
#include "olylumoray/rgba.h"

namespace olylumoray
{

class Hitable;

class Scene final
{
public:
    OLYLUMORAYAPI Scene();

    Hitable *
    world() const;

    const RGBA &
    environment_gradient_top() const;

    const RGBA &
    environment_gradient_bottom() const;

private:
    struct EnvironmentGradient
    {
        RGBA _top;
        RGBA _bottom;
    };
    Hitable            *_world;
    EnvironmentGradient _environment;
};

} // namespace olylumoray

#endif // OLYLUMORAY_SCENE_H
