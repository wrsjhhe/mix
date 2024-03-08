#pragma once

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <canvas/Canvas.h>
#include <renderers/common/Renderer.h>


namespace wgpu {
	class Instance;
	class Surface;
	class Adapter;
	class Device;
	class RenderPassDescriptor;
	class Texture;
	class TextureDescriptor;
	class SwapChain;
	class QuerySet;
	class Buffer;
	class FeatureName;
	class CommandEncoder;
	class RenderPassEncoder;
}

namespace mix {
	class WebGPURenderer;
	class WebGPUUtils;
	class WebGPUAttributeUtils;
	class WebGPUBindingUtils;
	class WebGPUPipelineUtils;
	class WebGPUTextureUtils;
	struct RenderContext;
	class RenderObject;
	class WGSLNodeBuilder;

	struct BackendResourceProperties
	{

	};

	struct BackendTextureResourceProperties : public BackendResourceProperties
	{
		bool initialized = false;
		std::shared_ptr<wgpu::Texture> texture = nullptr;
		std::shared_ptr<wgpu::Texture> msaaTexture = nullptr;
		std::shared_ptr<wgpu::TextureDescriptor> textureDescriptorGPU = nullptr;
	};

	struct BackendRenderContextResourceProperties : public BackendResourceProperties
	{
		std::shared_ptr<wgpu::QuerySet> occlusionQuerySet = nullptr;
		std::shared_ptr<wgpu::QuerySet> currentOcclusionQuerySet = nullptr;
		uint32_t occlusionQueryIndex = 0;

		std::shared_ptr<wgpu::Buffer> occlusionQueryBuffer = nullptr;
		std::shared_ptr<wgpu::Buffer> currentOcclusionQueryBuffer = nullptr;

		std::vector<RenderObject*> occlusionQueryObjects;
		std::vector<RenderObject*> currentOcclusionQueryObjects;

		RenderObject* lastOcclusionObject = nullptr;

		std::shared_ptr<wgpu::QuerySet> timeStampQuerySet = nullptr; 
		std::shared_ptr<wgpu::Buffer> currentTimestampQueryBuffer = nullptr;

		std::shared_ptr<wgpu::RenderPassDescriptor> descriptor = nullptr;
		std::shared_ptr<wgpu::CommandEncoder> encoder = nullptr;
		std::shared_ptr<wgpu::RenderPassEncoder> currentPass = nullptr;
		std::vector<wgpu::Buffer> attributes;

		std::unordered_set<RenderObject*> occluded;
	};

	struct BackendRenderTargetResourceProperties : public BackendResourceProperties
	{
		std::vector<std::shared_ptr<wgpu::RenderPassDescriptor>> descriptors;
		uint32_t width;
		uint32_t height;
		uint32_t activeMipmapLevel;
		uint32_t samples;
	};

	class WebGPUBackend{
	public:
		WebGPUBackend(const Renderer::Parameters& parameters = { });

		~WebGPUBackend();

		void init(Renderer* renderer);

		void updateSize();

		void beginRender(RenderContext* renderContext);
		void finishRender(RenderContext* renderContext);

		void draw(RenderObject* renderObject,Info& info);

		bool hasFeature(const wgpu::FeatureName& name );

		std::shared_ptr<WGSLNodeBuilder> createNodeBuilder(Object3D* object,Renderer* renderer,Scene* scene = nullptr);

		Canvas* getDomElement();

		Vector2& getDrawingBufferSize();

		void set(void*, const std::shared_ptr<BackendResourceProperties>&);
		std::shared_ptr<BackendResourceProperties> get(void*);
		bool has(void*);
		void remove(void*);
	private:
		std::shared_ptr<wgpu::RenderPassDescriptor> _getDefaultRenderPassDescriptor();

		std::shared_ptr<wgpu::RenderPassDescriptor> _getRenderPassDescriptor(RenderContext* renderContext);

		void initTimestampQuery(RenderContext* renderContext, wgpu::RenderPassDescriptor* descriptor);

		void updateViewport(RenderContext* renderContext);

		void resolveOccludedAsync(RenderContext* renderContext);

		void prepareTimestampBuffer(RenderContext* renderContext,wgpu::CommandEncoder* encoder);
	private:
		Renderer::Parameters parameters;
		//std::unordered_map<x, x> data;

		Renderer* renderer;

		std::unordered_map<void*, std::shared_ptr<BackendResourceProperties>> data;

		std::shared_ptr<wgpu::Instance> instance;
		std::shared_ptr <wgpu::Surface> surface;
		std::shared_ptr <wgpu::Adapter> adapter;
		std::shared_ptr <wgpu::Device> device;
		std::shared_ptr <wgpu::SwapChain> swapChain;
		std::shared_ptr<wgpu::RenderPassDescriptor> defaultRenderPassdescriptor;

		wgpu::Texture* colorBuffer;

		bool trackTimestamp = false;

		std::shared_ptr<WebGPUUtils> utils;
		std::shared_ptr<WebGPUAttributeUtils> attributeUtils;
		std::shared_ptr<WebGPUBindingUtils> bindingUtilstils;
		std::shared_ptr<WebGPUPipelineUtils> pipelineUtils;
		std::shared_ptr<WebGPUTextureUtils> textureUtils = nullptr;
		//occludedResolveCache

		std::unordered_map<uint32_t, std::shared_ptr<wgpu::Buffer>> occludedResolveCache;
	private:
		friend class WebGPUTextureUtils;
	};
}