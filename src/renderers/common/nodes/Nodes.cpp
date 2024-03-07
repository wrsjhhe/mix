#include <renderers/common/nodes/Nodes.h>
#include <renderers/common/Renderer.h>
#include <renderers/common/Backend.h>
#include <nodes/core/NodeFrame.h>
#include <nodes/lighting/EnvironmentNode.h>
#include <nodes/fog/FogNode.h>
#include <nodes/display/ToneMappingNode.h>
#include <scenes/Scene.h>
#include <utils/StringUtils.h>
#include <assert.h>

using namespace mix;

Nodes::Nodes(Renderer* renderer, Backend* backend)
	: renderer(renderer), backend(backend)
{
	nodeFrame = std::make_shared<NodeFrame>();
}

const std::string& Nodes::getCacheKey(Scene* scene, LightNode* lightsNode) {
	std::vector<void*> chain{ scene ,lightsNode };
	uint32_t callId = renderer->info.calls;

	CacheKeyData* cacheKeyData = reinterpret_cast<CacheKeyData*>(callHashCache.get(chain));
	if (cacheKeyData == nullptr || cacheKeyData->callId != callId) {
		EnvironmentNode* environmentNode = getEnvironmentNode(scene);
		FogNode* fogNode = getFogNode(scene);
		ToneMappingNode* toneMappingNode = getToneMappingNode();

		std::vector<std::string> cacheKey;
		if (lightsNode) cacheKey.emplace_back(lightsNode->getCacheKey());
		if (environmentNode) cacheKey.emplace_back(environmentNode->getCacheKey());
		if (fogNode) cacheKey.emplace_back(fogNode->getCacheKey());
		if (toneMappingNode) cacheKey.emplace_back(toneMappingNode->getCacheKey());

		std::shared_ptr<CacheKeyData> cacheKeyDataPtr = std::make_shared<CacheKeyData>();
		cacheKeyDataPtr->callId = callId;
		cacheKeyDataPtr->cacheKey = utils::join(cacheKey, ',');

		callHashCache.set(chain, cacheKeyDataPtr);
	}
	cacheKeyData = reinterpret_cast<CacheKeyData*>(callHashCache.get(chain));
	return cacheKeyData->cacheKey;
}

EnvironmentNode* Nodes::getEnvironmentNode(Scene* scene) {
	if (scene->environmentNode != nullptr) {
		return scene->environmentNode.get();
	}
	else if (get(scene) != nullptr) {
		return reinterpret_cast<EnvironmentNode*>(get(scene));
	}

	return nullptr;
}

FogNode* Nodes::getFogNode(Scene* scene) {
	if (scene->fogNode != nullptr) {
		return scene->fogNode.get();
	}
	else if (has(scene)) {
		return reinterpret_cast<FogNode*>(get(scene));
	}

	return nullptr;
}

ToneMappingNode* Nodes::getToneMappingNode() {
	if (!isToneMappingState) return nullptr;

	if (renderer->toneMappingNode != nullptr) {
		return renderer->toneMappingNode.get();
	}
	else if (has(renderer)) {
		assert(false);
		return nullptr;
	}
	return nullptr;

}