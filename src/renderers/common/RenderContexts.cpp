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

	auto renderState = chainMap.find(chainKey);
	if (renderState == chainMap.end()) {
		chainMap[chainKey] = std::shared_ptr<RenderContext>();
	}
	
	if (renderTarget != nullptr) renderState->second->sampleCount = renderTarget->samples == 0 ? 1 : renderTarget->samples;

	return renderState->second.get();
}

void  RenderContexts::dispose() {
	chainMaps.clear();
}