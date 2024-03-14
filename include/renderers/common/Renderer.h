#pragma once

#include <memory>
#include <vector>
#include <math/Plane.h>
#include <math/Vector2.h>
#include <math/Vector4.h>
#include <math/Matrix4.h>
#include <math/Color4.h>
#include <constants.h>
#include <renderers/common/Info.h>
#include <renderers/RenderTarget.h>
#include <optional>

namespace mix {
	class WebGPUBackend;
	class Object3D;
	class Scene;
	class Camera;
	class Canvas;
	class Attributes;
	class Geometries;
	class Nodes;
	class Animation;
	class Bindings;
	class RenderObjects;
	class Pipelines;
	class RenderList;
	class RenderLists;
	struct RenderContext;
	class RenderContexts;
	class Textures;
	class Background;
	class ToneMappingNode;

	class Renderer {
	public:
		struct Parameters
		{
			std::optional<bool> alpha;
			std::optional<bool> antialias;
			std::optional<uint32_t> sampleCount;
			std::optional<bool> trackTimestamp; 
		};
		Parameters parameters;

		Canvas* domElement = nullptr;

		std::vector<Plane> clippingPlanes;
		bool localClippingEnabled = false;

		Info info;

		std::shared_ptr<ToneMappingNode> toneMappingNode = nullptr;

		Renderer(const std::shared_ptr<WebGPUBackend>& backend, const Parameters& parameters);

		void init();

		void render(Scene* scene, Camera* camera);

		Vector2& getDrawingBufferSize(Vector2& target);

	private:
		void _projectObject(Object3D* object, Camera* camera, unsigned int groupOrder, RenderList* renderList);

	private:
		bool _initialized = false;

		Renderer* renderer = nullptr;
		std::shared_ptr<WebGPUBackend> backend = nullptr;

		bool logarithmicDepthBuffer = false;

		bool alpha = true;

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
	};
}