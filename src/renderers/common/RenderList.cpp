#include <renderers/common/RenderList.h>
#include <core/Object3D.h>
#include <core/BufferGeometry.h>
#include <core/misc.h>
#include <materials/Material.h>
#include <nodes/lighting/LightsNode.h>
#include <algorithm>

using namespace mix;

struct RenderList::RenderItem
{
	unsigned int id = INT_MIN;
	Object3D* object;
	BufferGeometry* geometry;
	Material* material;
	int groupOrder;
	unsigned int renderOrder;
	int z;
	GeometryGroup* group;
};

static bool painterSortStable(std::shared_ptr<RenderList::RenderItem>& a, std::shared_ptr<RenderList::RenderItem>& b) {

	if (a->groupOrder != b->groupOrder) {

		return a->groupOrder < b->groupOrder;

	}
	else if (a->renderOrder != b->renderOrder) {

		return a->renderOrder < b->renderOrder;

	}
	else if (a->material->id != b->material->id) {

		return a->material->id < b->material->id;

	}
	else if (a->z != b->z) {

		return a->z < b->z;

	}
	else {

		return a->id < b->id;

	}

}

static bool  reversePainterSortStable(std::shared_ptr<RenderList::RenderItem>& a, std::shared_ptr<RenderList::RenderItem>& b) {

	if (a->groupOrder != b->groupOrder) {

		return a->groupOrder < b->groupOrder;

	}
	else if (a->renderOrder != b->renderOrder) {

		return a->renderOrder < b->renderOrder;

	}
	else if (a->z != b->z) {

		return b->z < a->z;

	}
	else {

		return a->id < b->id;

	}

}


RenderList::RenderList():lightsNode(std::make_shared<LightsNode>()){

}

RenderList& RenderList::begin() {
	renderItemsIndex = 0;
}

std::shared_ptr<RenderList::RenderItem> RenderList::getNextRenderItem(Object3D* object,
	BufferGeometry* geometry,
	Material* material,
	int groupOrder,
	int z,
	GeometryGroup* group)
{
	if (renderItemsIndex >= renderItems.size()) {
		renderItems.emplace_back(new RenderItem{
			object->id,
			object,
			geometry,
			material,
			groupOrder,
			object->renderOrder,
			z,
			group
			});
	}

	auto renderItem = renderItems[renderItemsIndex];
	renderItemsIndex++;

	return renderItem;
}

void RenderList::push(Object3D* object,
	BufferGeometry* geometry,
	Material* material,
	int groupOrder,
	int z,
	GeometryGroup* group) 
{
	auto renderItem = getNextRenderItem(object, geometry, material, groupOrder, z, group);

	(material->transparent ? transparent : opaque).emplace_back(renderItem);
}

void RenderList::unshift(Object3D* object,
	BufferGeometry* geometry,
	Material* material,
	int groupOrder,
	int z,
	GeometryGroup* group)
{
	auto renderItem = getNextRenderItem(object, geometry, material, groupOrder, z, group);

	auto arr = (material->transparent ? transparent : opaque);
	arr.erase(std::remove(arr.begin(), arr.end(), renderItem), arr.end());
}

void RenderList::pushLight(Light* light) {
	lightsArray.emplace_back(light);
}

LightsNode* RenderList::getLightsNode() {
	return lightsNode->fromLights(lightsArray);
}

void RenderList::sort() {
	std::sort(opaque.begin(), opaque.end(), painterSortStable);
	std::sort(transparent.begin(), transparent.end(), reversePainterSortStable);
}

void RenderList::finish() {
	lightsNode->fromLights(lightsArray);

	for (int i = renderItemsIndex, il = renderItems.size(); i < il; ++i) {

		auto renderItem = renderItems[i];

		if (renderItem->id == INT_MIN) break;

		renderItem->id = INT_MIN;
		renderItem->object = nullptr;
		renderItem->geometry = nullptr;
		renderItem->material = nullptr;
		renderItem->groupOrder = INT_MIN;
		renderItem->renderOrder = INT_MIN;
		renderItem->z = INT_MIN;
		renderItem->group = nullptr;

	}
}