#include "olylumoray/scene.h"
#include "olylumoray/hitablelist.h"
#include "olylumoray/sphere.h"
#include "olylumoray/lambertian.h"
#include "olylumoray/metal.h"

namespace olylumoray
{

Scene::Scene()
{
    this->clear();
}

Scene::~Scene() = default;

Hitable *
Scene::world() const
{
    return this->_world.get();
}

const RGBA &
Scene::environment_gradient_top() const
{
    return this->_environment._top;
}

const RGBA &
Scene::environment_gradient_bottom() const
{
    return this->_environment._bottom;
}

void
Scene::clear()
{
    this->_world.reset(new HitableList);
    this->set_environment_gradient(
        RGBA(0.5f, 0.7f, 1.0f, 1.0f),
        RGBA(1, 1, 1, 1)
    );
}

void
Scene::set_environment_gradient(
    const RGBA &inTop,
    const RGBA &inBottom)
{
    this->_environment._top = inTop;
    this->_environment._bottom = inBottom;
}

void
Scene::append_sphere(
    const Vec4 &inPosition,
    const float inRadius,
    const std::string &inMaterialName,
    const RGBA &inAlbedo,
    const float inRoughness)
{
    Material *material = nullptr;
    if ("Lambertian" == inMaterialName)
    {
        material = new Lambertian(inAlbedo);
    }
    else if ("Metal" == inMaterialName)
    {
        material = new Metal(inAlbedo, inRoughness);
    }

    auto list = static_cast<HitableList*>(this->_world.get());
    list->append(new Sphere(
        inPosition,
        inRadius,
        material
    ));
}

} // namespace olylumoray
