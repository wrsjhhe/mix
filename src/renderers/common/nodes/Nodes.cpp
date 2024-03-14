#include <renderers/common/nodes/Nodes.h>
#include <renderers/common/Renderer.h>
#include <renderers/WebGPUBackend.h>
#include <renderers/common/RenderObject.h>
#include <renderers/common/nodes/NodeBuilderState.h>
#include <renderers/webgpu/nodes/WGSLNodeBuilder.h>
#include <nodes/core/NodeFrame.h>
#include <nodes/lighting/LightsNode.h>
#include <nodes/lighting/EnvironmentNode.h>
#include <nodes/fog/FogNode.h>
#include <nodes/display/ToneMappingNode.h>
#include <scenes/Scene.h>
#include <utils/StringUtils.h>
#include <assert.h>

using namespace mix;

Nodes::Nodes(Renderer* renderer, WebGPUBackend* backend)
	: renderer(renderer), backend(backend)
{
	nodeFrame = std::make_shared<NodeFrame>();
}

const std::string& Nodes::getCacheKey(Scene* scene, LightsNode* lightsNode) {
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
		cacheKeyDataPtr->cacheKey = utils::join(cacheKey, ",");

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

const std::string& Nodes::getForRenderCacheKey(RenderObject* renderObject) {
	return renderObject->initialCacheKey;
}

void Nodes::getForRender(RenderObject* renderObject) {
	RenderObjectData* renderObjectData = reinterpret_cast<RenderObjectData*>(get(renderObject));
	
	std::shared_ptr<NodeBuilderState>& nodeBuilderState = renderObjectData->nodeBuilderState;
	if (nodeBuilderState == nullptr) {
		const std::string& cacheKey = getForRenderCacheKey(renderObject);
		
		auto iter = nodeBuilderCache.find(cacheKey);
		if (iter == nodeBuilderCache.end()) {
			std::shared_ptr<WGSLNodeBuilder> nodeBuilder = backend->createNodeBuilder(renderObject->object,renderer, renderObject->scene);
			nodeBuilder->material = renderObject->material;
			nodeBuilder->context.material = renderObject->material;
			nodeBuilder->lightsNode = renderObject->lightsNode;
			nodeBuilder->environmentNode = getEnvironmentNode(renderObject->scene);
			nodeBuilder->fogNode = getFogNode(renderObject->scene);
			nodeBuilder->toneMappingNode = getToneMappingNode();
			nodeBuilder->clippingContext = renderObject->clippingContext.get();
			nodeBuilder->build();
		}
	}
}

void Nodes::updateScene(Scene* scene) {

}

void Nodes::updateEnvironment(Scene* scene) {
	auto sceneData = get(scene);

	
	
}

std::shared_ptr<NodeBuilderState> Nodes::_createNodeBuilderState(NodeBuilder* nodeBuilder) {
	new NodeBuilderState(
		nodeBuilder->vertexShader,
		nodeBuilder->fragmentShader,
		nodeBuilder->computeShader,
		nodeBuilder->getAttributesArray(),
		nodeBuilder->getBindings(),
		nodeBuilder->updateNodes,
		nodeBuilder->updateBeforeNodes,
		nodeBuilder->transforms
	);
}