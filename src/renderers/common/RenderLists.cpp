#include <renderers/common/RenderLists.h>
#include <scenes/Scene.h>
#include <cameras/Camera.h>

using namespace mix;

RenderList* RenderLists::get(const Scene* scene, const Camera* camera) {
	auto keys = std::pair<const Scene*, const Camera*>(scene, camera);

	auto iter = lists.find(keys);
	if (iter == lists.end()) {
		lists[keys] = std::make_shared<RenderList>();
	}

	auto list = lists[keys];
	return list.get();
}

void RenderLists::dispose() {
	lists.clear();
}