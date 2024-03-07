#pragma once

#include <unordered_map>
#include <memory>
#include <canvas/Canvas.h>
#include <renderers/common/Renderer.h>

namespace mix {
	struct RenderContext;
	class RenderObject;

	struct BackendResourceProperties
	{

	};

	class Backend {
	public:

	public:

		Backend(const Renderer::Parameters& parameters);

		virtual ~Backend();

		virtual void init(Renderer* renderer);

		// render context
		virtual void begin(std::shared_ptr<RenderContext>) {}
		virtual void finish(std::shared_ptr<RenderObject>) {}

		// render object
		virtual void draw(RenderObject* renderObject, Info& info) {}

		// program
		virtual void createProgram() {}
		virtual void destroyProgram() {}

		// bindings
		virtual void createBindings() {}
		virtual void updateBindings() {}

		// pipeline
		virtual void createRenderPipeline() {}
		virtual void createComputePipeline() {}
		virtual void destroyPipeline() {}

		// cache key
		virtual void needsRenderUpdate() {}
		virtual void getRenderCacheKey() {}

		// node builder
		virtual void createNodeBuilder() { } // return NodeBuilder (ADD IT)

		// textures
		virtual void createSampler() { }
		virtual void createDefaultTexture() { }
		virtual void createTexture() { }
		virtual void copyTextureToBuffer() {}

		// attributes
		virtual void createAttribute() { }
		virtual void createIndexAttribute() { }
		virtual void updateAttribute() { }
		virtual void destroyAttribute() { }

		// canvas
		virtual void getContext() { }
		virtual void updateSize() { }

		// utils
		virtual void resolveTimestampAsync() { }
		virtual void getInstanceCount() { }
		virtual Vector2& getDrawingBufferSize();
		virtual void getScissor() { }
		virtual void setScissorTest() { }
		virtual void getClearColor() { }
		virtual Canvas* getDomElement();

		// resource properties
		virtual void set(void*,const std::shared_ptr<BackendResourceProperties>&);
		virtual std::shared_ptr<BackendResourceProperties> get(void*);
		virtual bool has(void*);
		virtual void remove(void*);

	protected:
		Renderer::Parameters parameters;
		//std::unordered_map<x, x> data;

		Renderer* renderer;

		std::unordered_map<void*, std::shared_ptr<BackendResourceProperties>> data;
	};

}