#include <renderers/common/RenderContexts.h>
#include <scenes/Scene.h>
#include <cameras/Camera.h>
#include <renderers/RenderTarget.h>
#include <renderers/common/RenderContext.h>
#include <format>

using namespace mix;

RenderContext* RenderContexts::get(const Scene* scene, const Camera* camera, RenderTarget* renderTarget) {
	auto chainKey = std::pair<const Scene*, const Camera*>(scene, camera);
	std::string attachmentState = "";

	if (renderTarget == nullptr) {
		attachmentState = "default";
	}
	else {
		auto format = renderTarget->texture()->format;
		auto count = renderTarget->count;

		attachmentState = std::format("{}:{}:{}:{}:{}", count, (int)format, renderTarget->samples, renderTarget->depthBuffer, renderTarget->stencilBuffer);
	}

	auto iter = chainMaps.find(attachmentState);
	if (iter == chainMaps.end()) {
		chainMaps[attachmentState] = std::map<std::pair<const Scene*, const Camera*>, std::shared_ptr<RenderContext>>();
	}

	auto chainMap = chainMaps[attachmentState];

	auto iter2 = chainMap.find(chainKey);
	if (iter2 == chainMap.end()) {
		chainMap[chainKey] = std::make_shared<RenderContext>();
	}
	auto renderState = chainMap[chainKey];
	if (renderTarget != nullptr) renderState->sampleCount = renderTarget->samples == 0 ? 1 : renderTarget->samples;

	return renderState.get();
}

void  RenderContexts::dispose() {
	chainMaps.clear();
}