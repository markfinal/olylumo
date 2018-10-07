#include "olylumoray/scene.h"

namespace olylumoray
{

Scene::Scene()
{
    this->_world = nullptr;
    this->_environment._top = RGBA(0.5f, 0.7f, 1.0f, 1.0f);
    this->_environment._bottom = RGBA(1, 1, 1, 1);
}

Hitable *
Scene::world() const
{
    return this->_world;
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

} // namespace olylumoray
