#include <canvas/Canvas.h>
#include <constants.h>
#include <math/Vector4.h>
#include <math/Color4.h>
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

#include <renderers/RenderTarget.h>

#include <GLFW/glfw3.h>
#include <renderers/common/Renderer.h>
#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>
#include <glfw3webgpu.h>

using namespace mix;
using namespace wgpu;

struct Renderer::Impl {
	std::shared_ptr<Backend> backend = nullptr;
	std::shared_ptr<Canvas> domElement = nullptr;

	Instance m_instance = nullptr;
	Surface m_surface = nullptr;
	Adapter m_adapter = nullptr;
	Device m_device = nullptr;
	Queue m_queue = nullptr;

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

	Impl(const std::shared_ptr<Backend>& backend, const Parameters& parameters) : backend(backend), domElement(backend->getDomElement()) {

	}

	~Impl() {
		m_device.release();
		m_adapter.release();
		m_surface.release();
		m_instance.release();
	}
};

Renderer::Renderer(const std::shared_ptr<Backend>& backend, const Parameters& parameters) :pimpl_(std::make_shared<Impl>(backend, parameters)) {

}

void Renderer::render() {

}