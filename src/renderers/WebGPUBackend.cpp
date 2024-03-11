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
#include <renderers/common/Pipeline.h>
#include <renderers/webgpu/nodes/WGSLNodeBuilder.h>

#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>
#include <glfw3webgpu.h>

using namespace mix;
using namespace wgpu;

static Vector2 vector2;

static TextureViewDescriptor _defaultColorTextureViewDescriptor() {
	TextureViewDescriptor descriptor;
	descriptor.aspect = TextureAspect::All;
	descriptor.baseArrayLayer = 0;
	descriptor.arrayLayerCount = 1;
	descriptor.baseMipLevel = 0;
	descriptor.mipLevelCount = 1;
	descriptor.dimension = TextureViewDimension::_2D;
	descriptor.format = TextureFormat::RGBA8Unorm;
	return descriptor;
}

static TextureViewDescriptor _defaultDepthStencilTextureViewDescriptor() {
	TextureViewDescriptor descriptor;
	descriptor.aspect = TextureAspect::DepthOnly;
	descriptor.baseArrayLayer = 0;
	descriptor.arrayLayerCount = 1;
	descriptor.baseMipLevel = 0;
	descriptor.mipLevelCount = 1;
	descriptor.dimension = TextureViewDimension::_2D;
	descriptor.format = TextureFormat::Depth24Plus;
	return descriptor;
}


WebGPUBackend::WebGPUBackend(const Renderer::Parameters& parameters ):parameters(parameters) {

	// some parameters require default values other than "undefined"
	this->parameters.alpha = (!parameters.alpha.has_value()) ? true : parameters.alpha;
	this->parameters.antialias = (!parameters.antialias.has_value()) ? false : parameters.alpha;

	if (parameters.antialias == true) {
		this->parameters.sampleCount = (!parameters.sampleCount.has_value()) ? 4 : parameters.sampleCount.value();
	}
	else {
		this->parameters.sampleCount = 1;
	}
	this->trackTimestamp = parameters.trackTimestamp == true;
}

WebGPUBackend::~WebGPUBackend() {

	device->release();
	adapter->release();
	surface->release();
	instance->release();
}

void WebGPUBackend::init(Renderer* renderer) {
	this->renderer = renderer;

	textureUtils = std::make_shared<WebGPUTextureUtils>(this);
	utils = std::make_shared<WebGPUUtils>();
	attributeUtils = std::make_shared<WebGPUAttributeUtils>();
	bindingUtilstils = std::make_shared<WebGPUBindingUtils>();
	pipelineUtils = std::make_shared<WebGPUPipelineUtils>();

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
	BackendRenderContextResourceProperties* renderContextData = static_cast<BackendRenderContextResourceProperties*>(get(renderContext).get());
	uint32_t occlusionQueryCount = renderContext->occlusionQueryCount;

	std::shared_ptr<QuerySet> occlusionQuerySet;

	if(occlusionQueryCount > 0) {
		if (renderContextData->currentOcclusionQuerySet) renderContextData->currentOcclusionQuerySet->destroy();
		if (renderContextData->currentOcclusionQueryBuffer) renderContextData->currentOcclusionQueryBuffer->destroy();

		// Get a reference to the array of objects with queries. The renderContextData property
		// can be changed by another render pass before the buffer.mapAsyc() completes.
		renderContextData->currentOcclusionQuerySet = renderContextData->occlusionQuerySet;
		renderContextData->currentOcclusionQueryBuffer = renderContextData->occlusionQueryBuffer;
		renderContextData->currentOcclusionQueryObjects.swap(renderContextData->occlusionQueryObjects);

		QuerySetDescriptor querySetDescriptor{};
		querySetDescriptor.type = QueryType::Occlusion;
		querySetDescriptor.count = occlusionQueryCount;
		occlusionQuerySet = std::make_shared<QuerySet>(device->createQuerySet(querySetDescriptor));

		renderContextData->occlusionQuerySet = occlusionQuerySet;
		renderContextData->occlusionQueryIndex = 0;
		renderContextData->occlusionQueryObjects.clear();
		renderContextData->occlusionQueryObjects.resize(occlusionQueryCount);

		renderContextData->lastOcclusionObject = nullptr;
	}

	std::shared_ptr<RenderPassDescriptor> descriptor = nullptr;

	if (renderContext->textures.empty()) {
		descriptor = _getDefaultRenderPassDescriptor();
	}
	else {
		descriptor = _getRenderPassDescriptor(renderContext);
	}

	initTimestampQuery(renderContext,descriptor.get());
	descriptor->occlusionQuerySet = *occlusionQuerySet;
	WGPURenderPassDepthStencilAttachment* depthStencilAttachment = const_cast<WGPURenderPassDepthStencilAttachment*>(descriptor->depthStencilAttachment);

	if (renderContext->textures.empty()) {
		WGPURenderPassColorAttachment* colorAttachments = const_cast<WGPURenderPassColorAttachment*>(descriptor->colorAttachments);

		for (size_t i = 0; i < descriptor->colorAttachmentCount; ++i) {
			WGPURenderPassColorAttachment* colorAttachment = colorAttachments + i;
			if (renderContext->clearColor) {
				colorAttachment->clearValue = WGPUColor{ 
					renderContext->clearColorValue.r,
					renderContext->clearColorValue.g, 
					renderContext->clearColorValue.b, 
					renderContext->clearColorValue.a
				};
				colorAttachment->loadOp = LoadOp::Clear;
				colorAttachment->storeOp = StoreOp::Store;
			}
			else {
				colorAttachment->loadOp = LoadOp::Load;
				colorAttachment->storeOp = StoreOp::Store;
			}
		}
	}
	else {
		WGPURenderPassColorAttachment* colorAttachment = const_cast<WGPURenderPassColorAttachment*>(descriptor->colorAttachments);

		if (renderContext->clearColor) {

			colorAttachment->clearValue = WGPUColor{
					renderContext->clearColorValue.r,
					renderContext->clearColorValue.g,
					renderContext->clearColorValue.b,
					renderContext->clearColorValue.a
			};
			colorAttachment->loadOp = LoadOp::Clear;
			colorAttachment->storeOp = StoreOp::Store;
		}
		else {
			colorAttachment->loadOp = LoadOp::Load;
			colorAttachment->storeOp = StoreOp::Store;
		}
	}

	if (renderContext->depth) {
		if (renderContext->clearDepth) {
			depthStencilAttachment->depthClearValue = renderContext->clearDepthValue;
			depthStencilAttachment->depthLoadOp = LoadOp::Clear;
			depthStencilAttachment->depthStoreOp = StoreOp::Store;
		}
		else {
			depthStencilAttachment->depthLoadOp = LoadOp::Load;
			depthStencilAttachment->depthStoreOp = StoreOp::Store;
		}
	}

	if (renderContext->stencil) {

		if (renderContext->clearStencil) {
			depthStencilAttachment->stencilClearValue = renderContext->clearStencilValue;
			depthStencilAttachment->stencilLoadOp = LoadOp::Clear;
			depthStencilAttachment->stencilStoreOp = StoreOp::Store;
		}
		else {
			depthStencilAttachment->stencilLoadOp = LoadOp::Load;
			depthStencilAttachment->stencilStoreOp = StoreOp::Store;
		}
	}
	CommandEncoderDescriptor commandEncoderDescriptor{};
	commandEncoderDescriptor.label = (std::string("renderContext_") + std::to_string(renderContext->id)).c_str();
	std::shared_ptr<CommandEncoder> encoder = std::make_shared<CommandEncoder>(device->createCommandEncoder(commandEncoderDescriptor));
	std::shared_ptr<RenderPassEncoder> currentPass = std::make_shared<RenderPassEncoder>(encoder->beginRenderPass(*descriptor));

	renderContextData->descriptor = descriptor;
	renderContextData->encoder = encoder;
	renderContextData->currentPass = currentPass;

	if (renderContext->viewport) {
		updateViewport(renderContext);
	}

	if (renderContext->scissor) {
		Vector4& scissorValue = renderContext->scissorValue;
		float x = scissorValue.x;
		float y = scissorValue.y;
		float width = scissorValue.width();
		float height = scissorValue.height();
		currentPass->setScissorRect(x, renderContext->height - height - y, width, height);

	}
}

void WebGPUBackend::finishRender(RenderContext* renderContext) {
	BackendRenderContextResourceProperties* renderContextData = static_cast<BackendRenderContextResourceProperties*>(get(renderContext).get());
	uint32_t occlusionQueryCount = renderContext->occlusionQueryCount;

	if (occlusionQueryCount > renderContextData->occlusionQueryIndex) {
		renderContextData->currentPass->endOcclusionQuery();
	}

	renderContextData->currentPass->end();
	if (occlusionQueryCount > 0) {

		uint32_t bufferSize = occlusionQueryCount * 8; // 8 byte entries for query results

		//

		std::shared_ptr<Buffer> queryResolveBuffer = occludedResolveCache[bufferSize];
		if (queryResolveBuffer == nullptr) {
			BufferDescriptor bufferDescriptor{};
			bufferDescriptor.size = bufferSize;
			bufferDescriptor.usage = BufferUsage::QueryResolve | BufferUsage::CopySrc;
			queryResolveBuffer = std::make_shared<Buffer>(device->createBuffer(bufferDescriptor));

			occludedResolveCache[bufferSize] = queryResolveBuffer;
		}

		//
		BufferDescriptor bufferDescriptor{};
		bufferDescriptor.size = bufferSize;
		bufferDescriptor.usage = BufferUsage::CopyDst | BufferUsage::MapRead;
		std::shared_ptr<Buffer> readBuffer = std::make_shared<Buffer>(device->createBuffer(bufferDescriptor));

		// two buffers required here - WebGPU doesn't allow usage of QUERY_RESOLVE & MAP_READ to be combined
		renderContextData->encoder->resolveQuerySet(*renderContextData->occlusionQuerySet, 0, occlusionQueryCount, *queryResolveBuffer, 0);
		renderContextData->encoder->copyBufferToBuffer(*queryResolveBuffer, 0, *readBuffer, 0, bufferSize);

		renderContextData->occlusionQueryBuffer = readBuffer;

		//

		resolveOccludedAsync(renderContext);

	}

	prepareTimestampBuffer(renderContext, renderContextData->encoder.get());

	CommandBufferDescriptor cmdBufferDescriptor{};
	cmdBufferDescriptor.label = "Command buffer";
	CommandBuffer command = renderContextData->encoder->finish(cmdBufferDescriptor);
	device->getQueue().submit(command);


	//

	if (!renderContext->textures.empty()) {
		std::vector<Texture*>& textures = renderContext->textures;
		for (size_t i = 0; i < textures.size(); ++i) {
			Texture* texture = textures[i];
			if (texture->generateMipmaps) {
				//textureUtils.generateMipmaps( texture );
				assert(false);
			}
		}
	}

}

void WebGPUBackend::draw(RenderObject* renderObject, Info& info) {
	Object3D* object = renderObject->object;
	BufferGeometry* geometry = renderObject->geometry;
	RenderContext* renderContext = renderObject->context;
	Pipeline* pipeline = renderObject->pipeline;

	//auto bindingsData = get(renderObject->getBindings());
	//auto contextData = get(context);
	//auto pipelineGPU = get(pipeline).pipeline;
	//auto currentSets = contextData.currentSets;

	// pipeline
	

}

void WebGPUBackend::resolveOccludedAsync(RenderContext* renderContext) {
	BackendRenderContextResourceProperties* renderContextData = static_cast<BackendRenderContextResourceProperties*>(get(renderContext).get());
	std::shared_ptr<wgpu::Buffer>& currentOcclusionQueryBuffer = renderContextData->currentOcclusionQueryBuffer;
	std::vector<RenderObject*>& currentOcclusionQueryObjects = renderContextData->currentOcclusionQueryObjects;

	if (currentOcclusionQueryBuffer != nullptr && !currentOcclusionQueryObjects.empty()) {
		std::unordered_set<RenderObject*>& occluded = renderContextData->occluded;
		occluded.clear();

		renderContextData->currentOcclusionQueryObjects.clear();
		renderContextData->currentOcclusionQueryBuffer = nullptr;

		currentOcclusionQueryBuffer->mapAsync(MapMode::Read, 0, currentOcclusionQueryBuffer->getSize(), nullptr);
		uint64_t* results = (uint64_t*)currentOcclusionQueryBuffer->getMappedRange(0, currentOcclusionQueryBuffer->getSize());
		
		for (size_t i = 0; i < currentOcclusionQueryObjects.size(); ++i) {
			if (results[i] != 0) {
				occluded.emplace(currentOcclusionQueryObjects[i]);
			}
		}
		currentOcclusionQueryBuffer->destroy();
	}
}

void WebGPUBackend::prepareTimestampBuffer(RenderContext* renderContext, wgpu::CommandEncoder* encoder) {
	if (!hasFeature(FeatureName::TimestampQuery) || !trackTimestamp) return;

	BackendRenderContextResourceProperties* renderContextData = static_cast<BackendRenderContextResourceProperties*>(get(renderContext).get());

	constexpr uint32_t size = sizeof(int64_t);
	BufferDescriptor resolveBufferDescriptor;
	resolveBufferDescriptor.size = size;
	resolveBufferDescriptor.usage = BufferUsage::QueryResolve | BufferUsage::CopySrc;
	Buffer resolveBuffer = device->createBuffer(resolveBufferDescriptor);

	BufferDescriptor resultBufferDescriptor;
	resultBufferDescriptor.size = size;
	resultBufferDescriptor.usage = BufferUsage::CopyDst | BufferUsage::MapRead;
	std::shared_ptr<Buffer> resultBuffer = std::make_shared<Buffer>(device->createBuffer(resultBufferDescriptor));

	encoder->resolveQuerySet(*renderContextData->timeStampQuerySet, 0, 2, resolveBuffer, 0);
	encoder->copyBufferToBuffer(resolveBuffer, 0, *resultBuffer, 0, size);

	resolveBuffer.destroy();
	if (renderContextData->currentTimestampQueryBuffer != nullptr) {
		renderContextData->currentTimestampQueryBuffer->destroy();
	}

	renderContextData->currentTimestampQueryBuffer = resultBuffer;
}

void WebGPUBackend::updateViewport(RenderContext* renderContext) {
	BackendRenderContextResourceProperties* renderContextData = static_cast<BackendRenderContextResourceProperties*>(get(renderContext).get());
	RenderPassEncoder* currentPass = renderContextData->currentPass.get();
	Vector4& viewportValue =  renderContext->viewportValue;
	float x = viewportValue.x;
	float y = viewportValue.y;
	float width = viewportValue.width();
	float height = viewportValue.height();
	float minDepth = viewportValue.minDepth.value();
	float maxDepth = viewportValue.maxDepth.value();

	currentPass->setViewport(x, renderContext->height - height - y, width, height, minDepth, maxDepth);
}

void WebGPUBackend::initTimestampQuery(RenderContext* renderContext, wgpu::RenderPassDescriptor* descriptor) {
	if (!hasFeature(FeatureName::TimestampQuery) || !trackTimestamp) return;

	BackendRenderContextResourceProperties* renderContextData = static_cast<BackendRenderContextResourceProperties*>(get(renderContext).get());
	if (renderContextData->timeStampQuerySet == nullptr) {

		// Create a GPUQuerySet which holds 2 timestamp query results: one for the
		// beginning and one for the end of compute pass execution.
		QuerySetDescriptor querySetDescriptor{};
		querySetDescriptor.label = "timestamp";
		querySetDescriptor.count = 2;
		renderContextData->timeStampQuerySet = std::make_shared<QuerySet>(device->createQuerySet(querySetDescriptor));

		WGPURenderPassTimestampWrite* timestampWrites = new WGPURenderPassTimestampWrite({
			*renderContextData->timeStampQuerySet,
			0, // Write timestamp in index 0 when pass begins.
			RenderPassTimestampLocation::End, // Write timestamp in index 1 when pass ends.
			});

		descriptor->timestampWrites = timestampWrites;
	}
}

bool WebGPUBackend::hasFeature(const wgpu::FeatureName& name) {
	return adapter->hasFeature(name);
}

std::shared_ptr<WGSLNodeBuilder> WebGPUBackend::createNodeBuilder(Object3D* object, Renderer* renderer, Scene* scene) {
	return std::make_shared<WGSLNodeBuilder>(object, renderer, scene);
}

std::shared_ptr<RenderPassDescriptor> WebGPUBackend::_getDefaultRenderPassDescriptor() {
	std::shared_ptr<RenderPassDescriptor> descriptor = defaultRenderPassdescriptor;
	bool antialias = this->renderer->parameters.antialias.value();

	if (descriptor == nullptr) {
		descriptor = std::make_shared<RenderPassDescriptor>();

		RenderPassColorAttachment* colorAttachment = new RenderPassColorAttachment();
		colorAttachment->view = nullptr;
		descriptor->colorAttachmentCount = 1;
		descriptor->colorAttachments = colorAttachment;

		auto depthTexture = textureUtils->getDepthBuffer();
		TextureViewDescriptor depthTextureViewDesc;
		depthTextureViewDesc.aspect = TextureAspect::DepthOnly;
		depthTextureViewDesc.baseArrayLayer = 0;
		depthTextureViewDesc.arrayLayerCount = 1;
		depthTextureViewDesc.baseMipLevel = 0;
		depthTextureViewDesc.mipLevelCount = 1;
		depthTextureViewDesc.dimension = TextureViewDimension::_2D;
		depthTextureViewDesc.format = TextureFormat::Depth24Plus;

		RenderPassDepthStencilAttachment* depthStencilAttachment = new RenderPassDepthStencilAttachment();
		depthStencilAttachment->view = depthTexture->createView(depthTextureViewDesc);
		depthStencilAttachment->depthClearValue = 1.0f;
		depthStencilAttachment->depthLoadOp = LoadOp::Clear;
		depthStencilAttachment->depthStoreOp = StoreOp::Store;
		depthStencilAttachment->depthReadOnly = false;
		depthStencilAttachment->stencilClearValue = 1;
		depthStencilAttachment->stencilLoadOp = LoadOp::Clear;
		depthStencilAttachment->stencilStoreOp = StoreOp::Store;
		depthStencilAttachment->stencilReadOnly = false;

		descriptor->depthStencilAttachment = depthStencilAttachment;
		descriptor->depthStencilAttachment = nullptr;
		descriptor->timestampWriteCount = 0;
		descriptor->timestampWrites = nullptr;

		if (antialias) {
			TextureViewDescriptor colorTextureViewDesc;
			colorTextureViewDesc.aspect = TextureAspect::All;
			colorTextureViewDesc.baseArrayLayer = 0;
			colorTextureViewDesc.arrayLayerCount = 1;
			colorTextureViewDesc.baseMipLevel = 0;
			colorTextureViewDesc.mipLevelCount = 1;
			colorTextureViewDesc.dimension = TextureViewDimension::_2D;
			colorTextureViewDesc.format = TextureFormat::RGBA8Unorm;
			colorAttachment->view = colorBuffer->createView(colorTextureViewDesc);
		}
		else {
			colorAttachment->resolveTarget = nullptr;
		}
		defaultRenderPassdescriptor = descriptor;
	}

	WGPURenderPassColorAttachment* colorAttachment = const_cast<WGPURenderPassColorAttachment*>(descriptor->colorAttachments);

	if (antialias) {
		colorAttachment->resolveTarget = swapChain->getCurrentTextureView();
	}
	else {
		colorAttachment->view = swapChain->getCurrentTextureView();
	}

	return descriptor;
}

std::shared_ptr<RenderPassDescriptor> WebGPUBackend::_getRenderPassDescriptor(RenderContext* renderContext) {
	RenderTarget* renderTarget = renderContext->renderTarget;
	BackendRenderTargetResourceProperties* renderTargetData = static_cast<BackendRenderTargetResourceProperties*>(get(renderTarget).get());

	std::vector<std::shared_ptr<RenderPassDescriptor>>& descriptors = renderTargetData->descriptors;
	
	if (renderTargetData->width != renderTarget->width ||
		renderTargetData->height != renderTarget->height ||
		renderTargetData->activeMipmapLevel != renderTarget->activeMipmapLevel ||
		renderTargetData->samples != renderTarget->samples
		) {
		descriptors.clear();
		descriptors.resize(renderContext->activeCubeFace,nullptr);
	}
	std::shared_ptr<RenderPassDescriptor> descriptor = descriptors[renderContext->activeCubeFace];

	if (descriptor == nullptr) {
		descriptor = std::make_shared<RenderPassDescriptor>(); 

		std::vector<Texture*> textures = renderContext->textures;
		RenderPassColorAttachment* colorAttachments = new RenderPassColorAttachment[textures.size()];

		for (size_t i = 0; i < textures.size(); ++i) {

			BackendTextureResourceProperties* textureData = static_cast<BackendTextureResourceProperties*>(get(textures[i]).get());

			TextureViewDescriptor textureViewDescriptor{};
			textureViewDescriptor.baseMipLevel = renderContext->activeMipmapLevel;
			textureViewDescriptor.mipLevelCount = 1;
			textureViewDescriptor.baseArrayLayer = renderContext->activeCubeFace;
			textureViewDescriptor.dimension = WGPUTextureViewDimension::WGPUTextureViewDimension_2D;
			TextureView textureView = textureData->texture->createView(textureViewDescriptor);

			RenderPassColorAttachment colorAttachment = colorAttachments[i];
			colorAttachment.loadOp = LoadOp::Load;
			colorAttachment.storeOp = StoreOp::Store;
			if (textureData->msaaTexture != nullptr) {
				colorAttachment.view = textureData->msaaTexture->createView(_defaultColorTextureViewDescriptor());
				colorAttachment.resolveTarget = textureView;
			}
			else {
				colorAttachment.view = textureView;
				colorAttachment.resolveTarget = nullptr;
			}
		}

		BackendTextureResourceProperties* depthTextureData = static_cast<BackendTextureResourceProperties*>(get(renderContext->depthTexture).get());

		RenderPassDepthStencilAttachment* depthStencilAttachment = new RenderPassDepthStencilAttachment;

		depthStencilAttachment->view = depthTextureData->texture->createView(_defaultDepthStencilTextureViewDescriptor());

		descriptor->colorAttachmentCount = textures.size();
		descriptor->colorAttachments = colorAttachments;

		descriptor->depthStencilAttachment = depthStencilAttachment;

		descriptors[renderContext->activeCubeFace] = descriptor;

		renderTargetData->width = renderTarget->width;
		renderTargetData->height = renderTarget->height;
		renderTargetData->samples = renderTarget->samples;
		renderTargetData->activeMipmapLevel = renderTarget->activeMipmapLevel;

	}

	return descriptor;
}

Canvas* WebGPUBackend::getDomElement() {
	return renderer->domElement;
}

Vector2& WebGPUBackend::getDrawingBufferSize() {
	return renderer->getDrawingBufferSize(vector2);
}


void WebGPUBackend::set(void* object, const std::shared_ptr<BackendResourceProperties>& value) {
	data[object] = value;
}

std::shared_ptr<BackendResourceProperties> WebGPUBackend::get(void* object) {
	auto iter = data.find(object);
	if (iter != data.end()) {
		return iter->second;
	}

	return nullptr;
}

bool WebGPUBackend::has(void* object) {
	auto iter = data.find(object);
	return iter != data.end();
}

void WebGPUBackend::remove(void* object) {
	auto iter = data.find(object);
	if (iter != data.end()) {
		data.erase(iter);
	}
}