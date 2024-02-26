#include <renderers/WebGPUBackend.h>
#include <renderers/webgpu/utils/WebGPUUtils.h>
#include <renderers/webgpu/utils/WebGPUAttributeUtils.h>
#include <renderers/webgpu/utils/WebGPUBindingUtils.h>
#include <renderers/webgpu/utils/WebGPUPipelineUtils.h>
#include <renderers/webgpu/utils/WebGPUTextureUtils.h>
#include <renderers/WebGPURenderer.h>
#include <canvas/Canvas.h>

#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>
#include <glfw3webgpu.h>

using namespace mix;
using namespace wgpu;

struct WebGPUBackend::Impl {
	Instance instance = nullptr;
	Surface surface = nullptr;
	Adapter adapter = nullptr;
	Device device = nullptr;

	std::shared_ptr<Texture> colorBuffer;
	RenderPassDescriptor renderPassDesc;

	std::shared_ptr<WebGPUUtils> utils;
	std::shared_ptr<WebGPUAttributeUtils> attributeUtils;
	std::shared_ptr<WebGPUBindingUtils> ubindingUtilstils;
	std::shared_ptr<WebGPUPipelineUtils> pipelineUtils;
	std::shared_ptr<WebGPUTextureUtils> textureUtils;
	//occludedResolveCache

	Impl() {

	}

	~Impl() {
		device.release();
		adapter.release();
		surface.release();
		instance.release();
	}

	bool init(const Parameters& parameters,const std::shared_ptr<Canvas>& canvas) {
		instance = createInstance(InstanceDescriptor{});
		if (!instance) {
			std::cerr << "Could not initialize WebGPU!" << std::endl;
			return false;
		}


		surface = glfwGetWGPUSurface(instance, (GLFWwindow*)canvas->windowPtr());
		wgpu::RequestAdapterOptions adapterOpts;
		adapterOpts.compatibleSurface = surface;
		adapter = instance.requestAdapter(adapterOpts);



		return true;
	}

};


WebGPUBackend::WebGPUBackend(const Parameters& parameters ):Backend(parameters),pimpl_(std::make_shared<WebGPUBackend::Impl>()) {
	
}

void WebGPUBackend::init(std::shared_ptr<WebGPURenderer>& renderer) {
	Backend::init(renderer);
	pimpl_->init(parameters, this->getDomElement());
}