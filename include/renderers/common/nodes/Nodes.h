#pragma once
#include <memory>
#include <string>
#include <utils/ChainMap.h>
#include <utils/WeakMap.h>

namespace mix {
	class Renderer;
	class WebGPUBackend;
	class NodeFrame;
	class Scene;
	class LightsNode;
	class EnvironmentNode;
	class FogNode;
	class ToneMappingNode;
	class RenderObject;
	class NodeBuilderState;

	class Nodes : public DataMap {
	public:
		struct CacheKeyData
		{
			uint32_t callId;
			std::string cacheKey;
		};

		struct RenderObjectData
		{
			std::shared_ptr<NodeBuilderState> nodeBuilderState;
		};

	public:
		Nodes() = delete;
		Nodes(Renderer* renderer, WebGPUBackend* backend);


		std::shared_ptr<NodeFrame> nodeFrame;

		const std::string& getCacheKey(Scene* scene, LightsNode* lightsNode);

		EnvironmentNode* getEnvironmentNode(Scene* scene);

		FogNode* getFogNode(Scene* scene);

		ToneMappingNode* getToneMappingNode();

		const std::string& getForRenderCacheKey(RenderObject* renderObject);

		void getForRender(RenderObject* renderObject);
	protected:
		bool isToneMappingState = false;

	private:
		Renderer* renderer;
		WebGPUBackend* backend;

		ChainMap callHashCache;
		std::unordered_map<std::string, NodeBuilderState*> nodeBuilderCache;
	};
}