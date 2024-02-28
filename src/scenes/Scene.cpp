
#include <scenes/Scene.h>

using namespace mix;

std::string Scene::type() const
{
    return "Scene";
}

std::shared_ptr<Scene> Scene::create() {

    return std::make_shared<Scene>();
}


