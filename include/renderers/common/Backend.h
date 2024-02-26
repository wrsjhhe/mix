#pragma once

#include <unordered_map>
#include <memory>
#include <canvas/Canvas.h>

namespace mix {
	class Renderer;
	struct RenderContext;
	class RenderObject;

	class Backend {
	public:
		struct Parameters
		{
			bool alpha = true;
			bool antialias = false;
			bool sampleCount = 1;
			//requiredLimits
			bool trackTimestamp = true;

		};
		Backend(const Parameters& parameters);

		virtual ~Backend();

		virtual void init(const std::shared_ptr<Renderer>& renderer);

		// render context
		virtual void begin(std::shared_ptr<RenderContext>) {}
		virtual void finish(std::shared_ptr<RenderObject>) {}

		// render object
		virtual void draw(std::shared_ptr<RenderContext>) {}

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
		virtual void hasFeatureAsync() { } // return Boolean
		virtual void hasFeature() { } // return Boolean
		virtual void getInstanceCount() { }
		virtual void getDrawingBufferSize() { }
		virtual void getScissor() { }
		virtual void setScissorTest() { }
		virtual void getClearColor() { }
		virtual std::shared_ptr<Canvas> getDomElement();

		// resource properties
		virtual void set() { }
		virtual void get() { }
		virtual void has() { }
		virtual void remove() { }

	protected:
		Parameters parameters;
		//std::unordered_map<x, x> data;

		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Canvas> canvas;
	};

}