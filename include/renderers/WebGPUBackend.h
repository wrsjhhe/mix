#pragma once
#include <renderers/common/Backend.h>
#include <unordered_set>

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

	class WebGPUBackend : public Backend{
	public:
		WebGPUBackend(const Renderer::Parameters& parameters = { });

		virtual ~WebGPUBackend();

		virtual void init( Renderer* renderer) override;

		virtual void updateSize() override;

		void beginRender(RenderContext* renderContext);
		void finishRender(RenderContext* renderContext);

		void draw(RenderObject* renderObject,Info& info) override;

		virtual bool hasFeature(const wgpu::FeatureName& name );
	private:
		std::shared_ptr<wgpu::RenderPassDescriptor> _getDefaultRenderPassDescriptor();

		std::shared_ptr<wgpu::RenderPassDescriptor> _getRenderPassDescriptor(RenderContext* renderContext);

		void initTimestampQuery(RenderContext* renderContext, wgpu::RenderPassDescriptor* descriptor);

		void updateViewport(RenderContext* renderContext);

		void resolveOccludedAsync(RenderContext* renderContext);

		void prepareTimestampBuffer(RenderContext* renderContext,wgpu::CommandEncoder* encoder);
	private:
		WebGPUBackend* backend;

		std::shared_ptr<wgpu::Instance> instance = nullptr;
		std::shared_ptr <wgpu::Surface> surface = nullptr;
		std::shared_ptr <wgpu::Adapter> adapter = nullptr;
		std::shared_ptr <wgpu::Device> device = nullptr;
		std::shared_ptr <wgpu::SwapChain> swapChain = nullptr;
		std::shared_ptr<wgpu::RenderPassDescriptor> defaultRenderPassdescriptor;

		wgpu::Texture* colorBuffer;

		bool trackTimestamp = false;

		std::shared_ptr<WebGPUUtils> utils = std::make_shared<WebGPUUtils>();
		std::shared_ptr<WebGPUAttributeUtils> attributeUtils = std::make_shared<WebGPUAttributeUtils>();
		std::shared_ptr<WebGPUBindingUtils> ubindingUtilstils = std::make_shared<WebGPUBindingUtils>();
		std::shared_ptr<WebGPUPipelineUtils> pipelineUtils = std::make_shared<WebGPUPipelineUtils>();
		std::shared_ptr<WebGPUTextureUtils> textureUtils = nullptr;
		//occludedResolveCache

		std::unordered_map<uint32_t, std::shared_ptr<wgpu::Buffer>> occludedResolveCache;
	private:
		friend class WebGPUTextureUtils;
	};
}