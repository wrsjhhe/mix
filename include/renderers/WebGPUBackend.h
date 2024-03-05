#pragma once
#include <renderers/common/Backend.h>

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
}

namespace mix {
	class WebGPURenderer;
	class WebGPUUtils;
	class WebGPUAttributeUtils;
	class WebGPUBindingUtils;
	class WebGPUPipelineUtils;
	class WebGPUTextureUtils;
	class RenderContext;
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
	};

	class WebGPUBackend : public Backend{
	public:
		WebGPUBackend(const Renderer::Parameters& parameters = { });

		virtual ~WebGPUBackend();

		virtual void init( Renderer* renderer) override;

		virtual void updateSize() override;

		void beginRender(RenderContext* renderContext);

	private:
		std::shared_ptr<wgpu::RenderPassDescriptor> _getDefaultRenderPassDescriptor();

		std::shared_ptr<wgpu::RenderPassDescriptor> _getRenderPassDescriptor();
	private:
		WebGPUBackend* backend;

		std::shared_ptr<wgpu::Instance> instance = nullptr;
		std::shared_ptr <wgpu::Surface> surface = nullptr;
		std::shared_ptr <wgpu::Adapter> adapter = nullptr;
		std::shared_ptr <wgpu::Device> device = nullptr;
		std::shared_ptr <wgpu::SwapChain> swapChain = nullptr;
		std::shared_ptr<wgpu::RenderPassDescriptor> defaultRenderPassdescriptor;

		wgpu::Texture* colorBuffer;


		std::shared_ptr<WebGPUUtils> utils;
		std::shared_ptr<WebGPUAttributeUtils> attributeUtils;
		std::shared_ptr<WebGPUBindingUtils> ubindingUtilstils;
		std::shared_ptr<WebGPUPipelineUtils> pipelineUtils;
		std::shared_ptr<WebGPUTextureUtils> textureUtils;
		//occludedResolveCache
	private:
		friend class WebGPUTextureUtils;
	};
}