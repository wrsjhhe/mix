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
	class Texture;
	class NodeBuilder;

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

		struct SceneData
		{
			std::shared_ptr<Texture> environment;
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

		void updateScene(Scene* scene);

		void updateEnvironment(Scene* scene);
	protected:
		bool isToneMappingState = false;

		std::shared_ptr<NodeBuilderState> _createNodeBuilderState(NodeBuilder* nodeBuilder);

	private:
		Renderer* renderer;
		WebGPUBackend* backend;

		ChainMap callHashCache;
		std::unordered_map<std::string, NodeBuilderState*> nodeBuilderCache;
	};
}