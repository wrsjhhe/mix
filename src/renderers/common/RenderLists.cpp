#include <renderers/common/RenderLists.h>
#include <scenes/Scene.h>
#include <cameras/Camera.h>

using namespace mix;

RenderList* RenderLists::get(const Scene* scene, const Camera* camera) {
	std::vector<void*> keys;
	keys.push_back(const_cast<Scene*>(scene));
	keys.push_back(const_cast<Camera*>(camera));

	void* res = lists.get(keys);
	if (res == nullptr) {
		lists.set(keys, std::make_shared<RenderList>());
	}

	return  reinterpret_cast<RenderList*>(lists.get(keys));
}

void RenderLists::dispose() {
	lists.clear();
}