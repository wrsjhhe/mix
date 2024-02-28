#include <canvas/Canvas.h>
#include <constants.h>

#include <math/Vector4.h>
#include <math/Matrix4.h>
#include <math/Color4.h>
#include <math/Frustum.h>

#include <renderers/common/Info.h>
#include <renderers/common/Backend.h>
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

using namespace mix;

static Vector2 _drawingBufferSize;
static Vector4 _screen;
static Frustum _frustum;
static Vector3 _vector3;
static Matrix4 _projScreenMatrix;

struct Renderer::Impl {
	bool _initialized = false;

	Renderer* renderer = nullptr;
	std::shared_ptr<Backend> backend = nullptr;
	std::shared_ptr<Canvas> domElement = nullptr;

	bool logarithmicDepthBuffer = false;

	bool alpha = true;

	Canvas* canvas;

	bool autoClear = true;
	bool autoClearColor = true;
	bool autoClearDepth = true;
	bool autoClearStencil = true;

	ColorSpace outputColorSpace = ColorSpace::SRGBColorSpace;

	ToneMapping toneMapping = ToneMapping::None;
	float toneMappingExposure = 1.0f;

	bool sortObjects = true;

	bool depth = true;
	bool stencil = true;

	Info info;

	float _pixelRatio = 1;
	float _width = 0;
	float _height = 0;

	Vector4 _viewport;
	Vector4 _scissor;

	bool _scissorTest = false;

	std::shared_ptr<Attributes> _attributes = nullptr;
	std::shared_ptr<Geometries> _geometries = nullptr;
	std::shared_ptr<Nodes> _nodes = nullptr;
	std::shared_ptr<Animation> _animation = nullptr;
	std::shared_ptr<Bindings> _bindings = nullptr;
	std::shared_ptr<RenderObjects> _objects = nullptr;
	std::shared_ptr<Pipelines> _pipelines = nullptr;
	std::shared_ptr<RenderLists> _renderLists = nullptr;
	std::shared_ptr<RenderContexts> _renderContexts = nullptr;
	std::shared_ptr<Textures> _textures = nullptr;
	std::shared_ptr<Background> _background = nullptr;

	Color4 _clearColor = Color4();
	float _clearDepth = 1.0f;
	float _clearStencil = 0.0f;

	std::shared_ptr<RenderTarget> _renderTarget = nullptr;
	int _activeCubeFace = 0;
	int _activeMipmapLevel = 0;

	//_renderObjectFunction
	//_currentRenderObjectFunction
	struct ShadowMap
	{
		bool enabled = false;
		std::string type = "";
	};
	ShadowMap shadowMap;

	RenderContext* _currentRenderContext = nullptr;

	Impl(Renderer* renderer, const std::shared_ptr<Backend>& backend, const Parameters& parameters)
		:renderer(renderer), backend(backend), domElement(backend->getDomElement()) {

	}

	~Impl() {}

	void init() {
		_nodes = std::make_shared<Nodes>(renderer, backend.get());
		_renderContexts = std::make_shared<RenderContexts>();
		_renderLists = std::make_shared<RenderLists>();

		_initialized = true;
	}

	void render(Scene& scene, Camera& camera) {
		if (!_initialized) {
			init();
		}
		auto nodeFrame = _nodes->nodeFrame;

		const int previousRenderId = nodeFrame->renderId;
		const RenderContext* previousRenderContext = _currentRenderContext;
		//const previousRenderObjectFunction = this._currentRenderObjectFunction;

		auto renderTarget = _renderTarget;
		auto renderContext = _renderContexts->get(&scene, &camera, renderTarget.get());
		auto activeCubeFace = _activeCubeFace;
		auto activeMipmapLevel = _activeMipmapLevel;

		_currentRenderContext = renderContext;

		info.calls++;
		info.render.calls++;

		nodeFrame->renderId = info.calls;

		if (scene.matrixWorldNeedsUpdate == true) scene.updateMatrixWorld();

		if (camera.parent == nullptr && camera.matrixWorldAutoUpdate == true) camera.updateMatrixWorld();

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

		const unsigned int minDepth = 0;
		const unsigned int maxDepth = 1;

		renderContext->viewportValue.copy(viewport).multiplyScalar(pixelRatio).floor();
		int width = renderContext->viewportValue.z;
		int height = renderContext->viewportValue.w;
		width >>= activeMipmapLevel;
		height >>= activeMipmapLevel;
		renderContext->viewportValue.z = width;
		renderContext->viewportValue.w = height;
		renderContext->viewport = renderContext->viewportValue.equals(_screen) == false;

		renderContext->scissorValue.copy(scissor).multiplyScalar(pixelRatio).floor();
		renderContext->scissor = _scissorTest && renderContext->scissorValue.equals(_screen) == false;
		width = renderContext->scissorValue.z;
		height = renderContext->scissorValue.w;
		width >>= activeMipmapLevel;
		height >>= activeMipmapLevel;
		renderContext->scissorValue.z = width;
		renderContext->scissorValue.w = height;

		if (renderContext->clippingContext == nullptr) {
			renderContext->clippingContext = std::make_shared<ClippingContext>();
		}

		// sceneRef.onBeforeRender(this, scene, camera, renderTarget);
		auto renderList = _renderLists->get(&scene, &camera);

		renderList->begin();

		_projectObject(&scene,camera,0,renderList);

	}

	Vector2& getDrawingBufferSize(Vector2& target) {
		return target.set(_width * _pixelRatio, _height * _pixelRatio).floor();
	}

	void _projectObject(Object3D* object,Camera& camera,unsigned int groupOrder,RenderList* renderList) {
		if (object->visible == false) return;

		bool visible = object->layers.test(camera.layers);

		if (visible) {
			if (object->type() == "Group") {
				groupOrder = object->renderOrder;
			}
			else if (object->type() == "LOD") {
				auto lod = dynamic_cast<LOD*>(object);
				if (lod->autoUpdate == true) lod->update(camera);
			}
			else if (object->type() == "Light") {
				auto light = dynamic_cast<Light*>(object);
				renderList->pushLight(light);
			}
			else if (object->type() == "Sprite") {
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
			else if (object->type() == "LineLoop") {
			}
			else if (object->type() == "Mesh" || object->type() == "Line" || object->type() == "Points") {
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
};

Renderer::Renderer(const std::shared_ptr<Backend>& backend, const Parameters& parameters) :pimpl_(std::make_shared<Impl>(this, backend, parameters)) {

}



void Renderer::init() {

}

void Renderer::render(Scene& scene, Camera& camera) {
	pimpl_->render(scene, camera);
}