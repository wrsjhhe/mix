#pragma once
#include <memory>
#include <string>
#include <core/EventDispatcher.h>
namespace mix {
	class Nodes;
	class Geometries;
	class Renderer;
	class Object3D;
	class Material;
	class BufferGeometry;
	class Scene;
	class Camera;
	class LightsNode;
	struct RenderContext;
	class Attributes;
	class Pipeline;
	class ClippingContext;
	class Bindings;
	class NodeBuilderState;

	class RenderObject {
		struct OnMaterialDispose : EventListener {

			explicit OnMaterialDispose(RenderObject* scope) : scope_(scope) {}

			void onEvent(Event& event) override;

		private:
			RenderObject* scope_;
		};
	public:
		RenderObject(Nodes* nodes,
			Geometries* geometries,
			Renderer* renderer, Object3D* object,
			Material* material,
			Scene* scene, Camera* camera, LightsNode* lightsNode,
			RenderContext* renderContext);

		uint32_t id;

		Geometries* _geometries = nullptr;
		Renderer* renderer = nullptr;
		Object3D* object = nullptr;
		Material* material = nullptr;
		Scene* scene = nullptr;
		Camera* camera = nullptr;
		LightsNode* lightsNode = nullptr;
		RenderContext* context = nullptr;

		BufferGeometry* geometry;
		uint32_t version;
		Attributes* attributes = nullptr;
		Pipeline* pipeline = nullptr;

		std::shared_ptr<ClippingContext> clippingContext = nullptr;

		uint32_t clippingContextVersion;

		std::string initialNodesCacheKey;
		std::string initialCacheKey;


		void updateClipping(std::shared_ptr<ClippingContext> parent);

		std::string getNodesCacheKey();
		std::string getCacheKey();
		std::string getMaterialCacheKey();

		NodeBuilderState* getNodeBuilderState();
		Bindings* getBindings();


		void dispose();

		OnMaterialDispose onMaterialDispose;

	private:
		Nodes* _nodes = nullptr;
		std::shared_ptr<Bindings> _bindings;
		NodeBuilderState* _nodeBuilderState = nullptr;
	};
}