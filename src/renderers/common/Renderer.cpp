#include <canvas/Canvas.h>

#include <math/Frustum.h>

#include <renderers/common/Info.h>
#include <renderers/common/Attributes.h>
#include <renderers/common/Geometries.h>
#include <renderers/common/nodes/Nodes.h>
#include <renderers/common/Animation.h>
#include <renderers/common/Background.h>
#include <renderers/common/Textures.h>
#include <renderers/common/Pipelines.h>
#include <renderers/common/Bindings.h>
#include <renderers/common/RenderObjects.h>
#include <renderers/common/RenderLists.h>
#include <renderers/common/RenderContexts.h>
#include <renderers/common/RenderContext.h>

#include <renderers/WebGPUBackend.h>
#include <renderers/RenderTarget.h>

#include <nodes/core/NodeFrame.h>

#include <cameras/Camera.h>
#include <scenes/Scene.h>

#include <objects/Group.h>
#include <objects/LOD.h>
#include <objects/Sprite.h>

#include <lights/Light.h>

#include <GLFW/glfw3.h>
#include <renderers/common/Renderer.h>

#include <glfw3webgpu.h>
#include <iostream>

using namespace mix;

static Vector2 _drawingBufferSize;
static Vector4 _screen;
static Frustum _frustum;
static Vector3 _vector3;
static Matrix4 _projScreenMatrix;

Renderer::Renderer(const std::shared_ptr<WebGPUBackend>& backend, const Parameters& parameters) : parameters(parameters),backend(backend) {}


void Renderer::init() {
	backend->init(this);

	_nodes = std::make_shared<Nodes>(this, backend.get());
	_renderContexts = std::make_shared<RenderContexts>();
	_renderLists = std::make_shared<RenderLists>();

	_initialized = true;
}

void Renderer::render(Scene* scene, Camera* camera) {
	if (!_initialized) {
		init();
	}
	auto nodeFrame = _nodes->nodeFrame;

	const int previousRenderId = nodeFrame->renderId;
	const RenderContext* previousRenderContext = _currentRenderContext;
	//const previousRenderObjectFunction = this._currentRenderObjectFunction;

	auto renderTarget = _renderTarget;
	auto renderContext = _renderContexts->get(scene, camera, renderTarget.get());
	auto activeCubeFace = _activeCubeFace;
	auto activeMipmapLevel = _activeMipmapLevel;

	_currentRenderContext = renderContext;

	info.calls++;
	info.render.calls++;

	nodeFrame->renderId = info.calls;

	if (scene->matrixWorldNeedsUpdate == true) scene->updateMatrixWorld();

	if (camera->parent == nullptr && camera->matrixWorldAutoUpdate == true) camera->updateMatrixWorld();

	if (info.autoReset == true) info.reset();

	Vector4& viewport = _viewport;
	Vector4& scissor = _scissor;
	float pixelRatio = _pixelRatio;

	if (renderTarget != nullptr) {
		viewport = renderTarget->viewport;
		scissor = renderTarget->scissor;
		pixelRatio = 1;
	}
	getDrawingBufferSize(_drawingBufferSize);

	_screen.set(0, 0, _drawingBufferSize.x, _drawingBufferSize.y);

	float minDepth = viewport.minDepth.has_value() ? 0 : viewport.minDepth.value();
	float maxDepth = viewport.maxDepth.has_value() ? 1 : viewport.maxDepth.value();

	renderContext->viewportValue.copy(viewport).multiplyScalar(pixelRatio).floor();
	uint32_t width = renderContext->viewportValue.width();
	uint32_t height = renderContext->viewportValue.height();
	width >>= activeMipmapLevel;
	height >>= activeMipmapLevel;
	renderContext->viewportValue.setWidth(width);
	renderContext->viewportValue.setHeight(height);
	renderContext->viewportValue.minDepth = minDepth;
	renderContext->viewportValue.maxDepth = maxDepth;
	renderContext->viewport = renderContext->viewportValue.equals(_screen) == false;

	renderContext->scissorValue.copy(scissor).multiplyScalar(pixelRatio).floor();
	renderContext->scissor = _scissorTest && renderContext->scissorValue.equals(_screen) == false;
	width = renderContext->scissorValue.width();
	height = renderContext->scissorValue.height();
	width >>= activeMipmapLevel;
	height >>= activeMipmapLevel;
	renderContext->scissorValue.setWidth(width);
	renderContext->scissorValue.setHeight(height);

	if (renderContext->clippingContext == nullptr) {
		renderContext->clippingContext = std::make_shared<ClippingContext>();
	}

	// sceneRef.onBeforeRender(this, scene, camera, renderTarget);
	auto renderList = _renderLists->get(scene, camera);

	renderList->begin();

	_projectObject(scene, camera, 0, renderList);

	renderList->finish();

	if (sortObjects == true) {
		renderList->sort();
	}
	
	if (renderTarget != nullptr) {

	}
	else {
		renderContext->textures.clear();
		renderContext->depthTexture = nullptr;
		renderContext->width = domElement->size().width;
		renderContext->height = domElement->size().height;
		renderContext->depth = depth;
		renderContext->stencil = stencil;
	}

	renderContext->width >>= activeMipmapLevel;
	renderContext->height >>= activeMipmapLevel;
	renderContext->activeCubeFace = activeCubeFace;
	renderContext->activeMipmapLevel = activeMipmapLevel;
	renderContext->occlusionQueryCount = renderList->occlusionQueryCount;

	_nodes->updateScene(scene);
}


Vector2& Renderer::getDrawingBufferSize(Vector2& target) {
	return target.set(_width * _pixelRatio, _height * _pixelRatio).floor();
}

void Renderer::_projectObject(Object3D* object, Camera* camera, unsigned int groupOrder, RenderList* renderList) {
	if (object->visible == false) return;

	bool visible = object->layers.test(camera->layers);

	if (visible) {
		std::string objectType = object->type();

		if (objectType == "Group") {
			groupOrder = object->renderOrder;
		}
		else if (objectType == "LOD") {
			auto lod = dynamic_cast<LOD*>(object);
			if (lod->autoUpdate == true) lod->update(*camera);
		}
		else if (objectType == "Light") {
			auto light = dynamic_cast<Light*>(object);
			renderList->pushLight(light);
		}
		else if (objectType == "Sprite") {
			auto sprite = dynamic_cast<Sprite*>(object);
			if (!object->frustumCulled || _frustum.intersectsSprite(*sprite)) {
				if (sortObjects == true) {
					_vector3.setFromMatrixPosition(*object->matrixWorld).applyMatrix4(_projScreenMatrix);
				}

				auto geometry = sprite->geometry();
				auto material = sprite->material;
				if (material->visible) {
					renderList->push(object, geometry, material.get(), groupOrder, _vector3.z, nullptr);
				}
			}
		}
		/*	else if (object->type() == "LineLoop") {

			}*/
		else if (objectType == "Mesh" || objectType == "Line" || objectType == "Points") {
			auto geometry = object->geometry();
			auto materials = object->materials();

			if (sortObjects == true) {

				if (!geometry->boundingSphere.has_value()) geometry->computeBoundingSphere();

				_vector3
					.copy(geometry->boundingSphere->center)
					.applyMatrix4(*object->matrixWorld)
					.applyMatrix4(_projScreenMatrix);

			}

			if (materials.size() > 1) {

				auto groups = geometry->groups;

				for (size_t i = 0, l = groups.size(); i < l; ++i) {

					auto group = groups[i];
					auto groupMaterial = materials[group.materialIndex];

					if (groupMaterial && groupMaterial->visible) {

						renderList->push(object, geometry, groupMaterial, groupOrder, _vector3.z, &group);

					}

				}

			}
			else if (object->material()->visible) {

				renderList->push(object, geometry, object->material(), groupOrder, _vector3.z, nullptr);

			}
		}
	}

	auto children = object->children;

	for (size_t i = 0, l = children.size(); i < l; ++i) {

		_projectObject(children[i], camera, groupOrder, renderList);

	}
}