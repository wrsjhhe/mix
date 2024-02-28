#pragma once
#include <vector>
#include <memory>

namespace mix {
	class Object3D;
	class BufferGeometry;
	class Material;
	struct GeometryGroup;
	class Light;
	class LightsNode;

	class RenderList {
	public:
		struct RenderItem;

		std::vector<std::shared_ptr<RenderItem>> renderItems;
		unsigned int renderItemsIndex = 0;

		std::vector<std::shared_ptr<RenderItem>> opaque;
		std::vector<std::shared_ptr<RenderItem>> transparent;

		std::vector<Light*> lightsArray;
		std::shared_ptr<LightsNode> lightsNode;

		unsigned int occlusionQueryCount = 0;

		RenderList();

		RenderList& begin();

		std::shared_ptr<RenderItem> getNextRenderItem(Object3D* object,
			BufferGeometry* geometry,
			Material* material,
			int groupOrder,
			int z,
			GeometryGroup* group);

		void push(Object3D* object,
			BufferGeometry* geometry,
			Material* material,
			int groupOrder,
			int z,
			GeometryGroup* group);

		void unshift(Object3D* object,
			BufferGeometry* geometry,
			Material* material,
			int groupOrder,
			int z,
			GeometryGroup* group);

		void pushLight(Light* light);

		LightsNode* getLightsNode();

		void sort();

		void finish();
	};
}