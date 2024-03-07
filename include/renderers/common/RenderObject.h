#pragma once
#include <memory>
#include <string>

namespace mix {
	class Nodes;
	class Geometries;
	class Renderer;
	class Object3D;
	class Material;
	class Scene;
	class Camera;
	class LightNode;
	struct RenderContext;
	class Attributes;
	class Pipelines;
	class ClippingContext;

	class RenderObject {
	public:
		RenderObject(Nodes* nodes,
			Geometries* geometries,
			Renderer* renderer, Object3D* object,
			Material* material,
			Scene* scene, Camera* camera, LightNode* lightsNode,
			RenderContext* renderContext);

		uint32_t id;

		Nodes* _nodes = nullptr;
		Geometries* _geometries = nullptr;
		Renderer* renderer = nullptr;
		Object3D* object = nullptr;
		Material* material = nullptr;
		Scene* scene = nullptr;
		Camera* camera = nullptr;
		LightNode* lightsNode = nullptr;
		RenderContext* renderContext = nullptr;

		uint32_t version;
		Attributes* attributes = nullptr;
		Pipelines* pipeline = nullptr;

		std::shared_ptr<ClippingContext> clippingContext = nullptr;

		uint32_t clippingContextVersion;

		std::string initialNodesCacheKey;
		std::string initialCacheKey;


		void updateClipping(std::shared_ptr<ClippingContext> parent);

		std::string getNodesCacheKey();
		std::string getCacheKey();
		std::string getMaterialCacheKey();
	};
}