#pragma once
#include <memory>
#include <string>
#include <utils/ChainMap.h>
#include <utils/WeakMap.h>

namespace mix {
	class Renderer;
	class Backend;
	class NodeFrame;
	class Scene;
	class LightNode;
	class EnvironmentNode;
	class FogNode;
	class ToneMappingNode;
	class Nodes : public DataMap {
	public:
		struct CacheKeyData
		{
			uint32_t callId;
			std::string cacheKey;
		};

	public:
		Nodes() = delete;
		Nodes(Renderer* renderer, Backend* backend);


		std::shared_ptr<NodeFrame> nodeFrame;

		const std::string& getCacheKey(Scene* scene,LightNode* lightsNode);

		EnvironmentNode* getEnvironmentNode(Scene* scene);

		FogNode* getFogNode(Scene* scene);

		ToneMappingNode* getToneMappingNode();
	protected:
		bool isToneMappingState = false;

	private:
		Renderer* renderer;
		Backend* backend;

		ChainMap callHashCache;
	};
}