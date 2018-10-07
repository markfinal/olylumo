#include "olylumoray/scene.h"
#include "olylumoray/hitablelist.h"

namespace olylumoray
{

Scene::Scene()
{
    this->clear();
}

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

} // namespace olylumoray
