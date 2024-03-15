#include <renderers/common/RenderObject.h>
#include <renderers/common/nodes/Nodes.h>
#include <renderers/common/Geometries.h>
#include <renderers/common/Renderer.h>
#include <renderers/common/RenderContext.h>
#include <renderers/common/Bindings.h>
#include <renderers/common/nodes/NodeBuilderState.h>
#include <core/Object3D.h>
#include <materials/Material.h>
#include <scenes/Scene.h>
#include <cameras/Camera.h>
#include <nodes/lighting/LightsNode.h>
#include <renderers/common/ClippingContext.h>

using namespace mix;

static uint32_t _id = 0;

void RenderObject::OnMaterialDispose::onEvent(Event& event) {
	scope_->dispose();
}

RenderObject::RenderObject(Nodes* nodes,
	Geometries* geometries,
	Renderer* renderer, Object3D* object,
	Material* material,
	Scene* scene, Camera* camera, LightsNode* lightsNode,
	RenderContext* renderContext): id(_id++),
	_nodes(nodes),_geometries(geometries),renderer(renderer),object(object), material(material),
	scene(scene),camera(camera),lightsNode(lightsNode),geometry(object->geometry()), context(renderContext), onMaterialDispose(this)
{
	version = material->version;

	updateClipping(renderContext->clippingContext);

	clippingContextVersion = clippingContext->version;

	initialNodesCacheKey = getNodesCacheKey();
	initialCacheKey = getCacheKey();


	material->addEventListener("dispose", &onMaterialDispose);
}

void RenderObject::updateClipping(std::shared_ptr<ClippingContext> parent) {
	if (!material->clippingPlanes.empty()) {
		clippingContext = std::make_shared<ClippingContext>();
	}
	else if (clippingContext != parent) {
		clippingContext = parent;
	}
}

std::string RenderObject::getNodesCacheKey() {
	return _nodes->getCacheKey(scene, lightsNode);
}

std::string RenderObject::getCacheKey() {
	return getMaterialCacheKey() + "," + getNodesCacheKey();
}
std::string RenderObject::getMaterialCacheKey() {
	std::string cacheKey = material->customProgramCacheKey();

	//todo Material key by property
	return material->uuid();
}

NodeBuilderState* RenderObject::getNodeBuilderState() {
	if (_nodeBuilderState != nullptr) return _nodeBuilderState;

	return _nodeBuilderState = _nodes->getForRender(this);
}

Bindings* RenderObject::getBindings() {
	if (_bindings != nullptr) return _bindings.get();
	else {

	}
}

void RenderObject::dispose() {
	material->removeEventListener("dispose", &onMaterialDispose);

}