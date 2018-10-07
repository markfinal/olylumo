#ifndef OLYLUMORAY_SCENE_H
#define OLYLUMORAY_SCENE_H

#include "olylumoray/api.h"
#include "olylumoray/rgba.h"

#include <memory>
#include <string>

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

    OLYLUMORAYAPI void
    clear();

    OLYLUMORAYAPI void
    set_environment_gradient(
        const RGBA &inTop,
        const RGBA &inBottom);

    OLYLUMORAYAPI void
    append_sphere(
        const Vec4 &inPosition,
        const float inRadius,
        const std::string &inMaterialName,
        const RGBA &inAlbedo);

private:
    struct EnvironmentGradient
    {
        RGBA _top;
        RGBA _bottom;
    };
    std::unique_ptr<Hitable> _world;
    EnvironmentGradient _environment;
};

} // namespace olylumoray

#endif // OLYLUMORAY_SCENE_H
