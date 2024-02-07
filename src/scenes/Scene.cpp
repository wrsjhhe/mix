
#include <scenes/Scene.h>

using namespace mix;


std::shared_ptr<Scene> Scene::create() {

    return std::make_shared<Scene>();
}
