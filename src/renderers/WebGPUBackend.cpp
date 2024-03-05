#include <renderers/WebGPUBackend.h>
#include <renderers/webgpu/utils/WebGPUUtils.h>
#include <renderers/webgpu/utils/WebGPUAttributeUtils.h>
#include <renderers/webgpu/utils/WebGPUBindingUtils.h>
#include <renderers/webgpu/utils/WebGPUPipelineUtils.h>
#include <renderers/webgpu/utils/WebGPUTextureUtils.h>
#include <renderers/WebGPURenderer.h>
#include <canvas/Canvas.h>
#include <renderers/common/RenderContext.h>
#include <renderers/common/RenderObject.h>

#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>
#include <glfw3webgpu.h>

using namespace mix;
using namespace wgpu;


WebGPUBackend::WebGPUBackend(const Renderer::Parameters& parameters ):Backend(parameters) {
	
}

WebGPUBackend::~WebGPUBackend() {

	device->release();
	adapter->release();
	surface->release();
	instance->release();
}

void WebGPUBackend::init(Renderer* renderer) {
	Backend::init(renderer);

	instance = std::make_shared<Instance>(createInstance(InstanceDescriptor{}));
	if (!instance) {
		std::cerr << "Could not initialize WebGPU!" << std::endl;
		return;
	}

	surface = std::make_shared<Surface>(glfwGetWGPUSurface(*instance, (GLFWwindow*)getDomElement()->windowPtr()));

	wgpu::RequestAdapterOptions adapterOpts;
	adapterOpts.compatibleSurface = *surface;
	adapter = std::make_shared<Adapter>(instance->requestAdapter(adapterOpts));

	DeviceDescriptor deviceDesc;
	deviceDesc.label = "mix Device";
	deviceDesc.requiredFeaturesCount = 0;
	deviceDesc.requiredLimits = nullptr;
	deviceDesc.defaultQueue.label = "The default queue";
	device = std::make_shared<Device>(adapter->requestDevice(deviceDesc));

#ifdef WEBGPU_BACKEND_WGPU
	TextureFormat swapChainFormat = surface->getPreferredFormat(*adapter);
#else
	TextureFormat swapChainFormat = TextureFormat::BGRA8Unorm;
#endif
	SwapChainDescriptor swapChainDesc;
	swapChainDesc.width = 640;
	swapChainDesc.height = 480;
	swapChainDesc.usage = TextureUsage::RenderAttachment;
	swapChainDesc.format = swapChainFormat;
	swapChainDesc.presentMode = PresentMode::Fifo;
	swapChain = std::make_shared<SwapChain>(device->createSwapChain(*surface, swapChainDesc));

	updateSize();
}


void WebGPUBackend::updateSize() {
	colorBuffer = textureUtils->getColorBuffer();
	defaultRenderPassdescriptor = nullptr;
}

void WebGPUBackend::beginRender(RenderContext* renderContext) {
	auto data = get(renderContext).get();
	auto renderContextData = static_cast<BackendRenderContextResourceProperties*>(data);

	auto occlusionQueryCount = renderContext->occlusionQueryCount;

	std::shared_ptr<QuerySet> occlusionQuerySet;

	if(occlusionQueryCount > 0) {
		if (renderContextData->currentOcclusionQuerySet) renderContextData->currentOcclusionQuerySet->destroy();
		if (renderContextData->currentOcclusionQueryBuffer) renderContextData->currentOcclusionQueryBuffer->destroy();

		// Get a reference to the array of objects with queries. The renderContextData property
		// can be changed by another render pass before the buffer.mapAsyc() completes.
		renderContextData->currentOcclusionQuerySet = renderContextData->occlusionQuerySet;
		renderContextData->currentOcclusionQueryBuffer = renderContextData->occlusionQueryBuffer;
		renderContextData->currentOcclusionQueryObjects = renderContextData->occlusionQueryObjects;

		QuerySetDescriptor querySetDescriptor{};
		querySetDescriptor.type = WGPUQueryType::WGPUQueryType_Occlusion;
		querySetDescriptor.count = occlusionQueryCount;
		occlusionQuerySet = std::make_shared<QuerySet>(device->createQuerySet(querySetDescriptor));

		renderContextData->occlusionQuerySet = occlusionQuerySet;
		renderContextData->occlusionQueryIndex = 0;
		renderContextData->occlusionQueryObjects.clear();
		renderContextData->occlusionQueryObjects.resize(occlusionQueryCount);

		renderContextData->lastOcclusionObject = nullptr;
	}
}

std::shared_ptr<RenderPassDescriptor> WebGPUBackend::_getDefaultRenderPassDescriptor() {
	auto descriptor = defaultRenderPassdescriptor;
	bool antialias = backend->renderer->parameters.antialias;

	if (descriptor == nullptr) {
		descriptor = std::make_unique<RenderPassDescriptor>();
		RenderPassDescriptor descriptor;

		RenderPassColorAttachment colorAttachment;
		colorAttachment.view = nullptr;
		descriptor.colorAttachmentCount = 1;
		descriptor.colorAttachments = &colorAttachment;

		auto depthTexture = textureUtils->getDepthBuffer();
		TextureViewDescriptor depthTextureViewDesc;
		depthTextureViewDesc.aspect = TextureAspect::DepthOnly;
		depthTextureViewDesc.baseArrayLayer = 0;
		depthTextureViewDesc.arrayLayerCount = 1;
		depthTextureViewDesc.baseMipLevel = 0;
		depthTextureViewDesc.mipLevelCount = 1;
		depthTextureViewDesc.dimension = TextureViewDimension::_2D;
		depthTextureViewDesc.format = TextureFormat::Depth24Plus;

		RenderPassDepthStencilAttachment depthStencilAttachment;
		depthStencilAttachment.view = depthTexture->createView(depthTextureViewDesc);
		depthStencilAttachment.depthClearValue = 1.0f;
		depthStencilAttachment.depthLoadOp = LoadOp::Clear;
		depthStencilAttachment.depthStoreOp = StoreOp::Store;
		depthStencilAttachment.depthReadOnly = false;
		depthStencilAttachment.stencilClearValue = 0;
#ifdef WEBGPU_BACKEND_WGPU
		depthStencilAttachment.stencilLoadOp = LoadOp::Clear;
		depthStencilAttachment.stencilStoreOp = StoreOp::Store;
#else
		depthStencilAttachment.stencilLoadOp = LoadOp::Undefined;
		depthStencilAttachment.stencilStoreOp = StoreOp::Undefined;
#endif
		depthStencilAttachment.stencilReadOnly = true;

		descriptor.depthStencilAttachment = &depthStencilAttachment;
		descriptor.depthStencilAttachment = nullptr;
		descriptor.timestampWriteCount = 0;
		descriptor.timestampWrites = nullptr;

		if (antialias) {
			TextureViewDescriptor colorTextureViewDesc;
			colorAttachment.view = colorBuffer->createView(colorTextureViewDesc);
		}
		else {
			colorAttachment.resolveTarget = nullptr;
		}
		defaultRenderPassdescriptor = std::make_shared<RenderPassDescriptor>();
		*defaultRenderPassdescriptor = descriptor;
	}

	auto colorAttachment = descriptor->colorAttachments[0];

	if (antialias) {
		colorAttachment.resolveTarget = swapChain->getCurrentTextureView();
	}
	else {
		colorAttachment.view = swapChain->getCurrentTextureView();
	}

	return descriptor;
}

std::shared_ptr<wgpu::RenderPassDescriptor> WebGPUBackend::_getRenderPassDescriptor() {

}