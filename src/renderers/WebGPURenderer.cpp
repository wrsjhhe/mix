#include <canvas/canvas.h>
#include <GLFW/glfw3.h>
#include <renderers/WebGPURenderer.h>
#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>
#include <glfw3webgpu.h>



using namespace mix;
using namespace wgpu;

struct WebGPURenderer::Impl {
	Instance m_instance = nullptr;
	Surface m_surface = nullptr;
	Adapter m_adapter = nullptr;
	Device m_device = nullptr;
	Queue m_queue = nullptr;

	Impl(Canvas& canvas) {
		m_instance = createInstance(InstanceDescriptor{});
		auto pInstance = &m_instance;
		auto window = (GLFWwindow*)(canvas.windowPtr());

		m_surface = glfwGetWGPUSurface(*pInstance, window);
		RequestAdapterOptions adapterOpts;
		adapterOpts.compatibleSurface = m_surface;
		m_adapter = pInstance->requestAdapter(adapterOpts);

		DeviceDescriptor deviceDesc;
		deviceDesc.label = "My Device";
		deviceDesc.requiredFeaturesCount = 0;
		deviceDesc.requiredLimits = nullptr;
		deviceDesc.defaultQueue.label = "The default queue";
		m_device = m_adapter.requestDevice(deviceDesc);

		auto h = m_device.setUncapturedErrorCallback([](ErrorType type, char const* message) {
			std::cout << "Device error: type " << type;
			if (message) std::cout << " (message: " << message << ")";
			std::cout << std::endl;
			});

		m_queue = m_device.getQueue();


#ifdef WEBGPU_BACKEND_WGPU
		TextureFormat swapChainFormat = m_surface.getPreferredFormat(m_adapter);
#else
		TextureFormat swapChainFormat = TextureFormat::BGRA8Unorm;
#endif
		SwapChainDescriptor swapChainDesc;
		swapChainDesc.width = canvas.size().width;
		swapChainDesc.height = canvas.size().height;
		swapChainDesc.usage = TextureUsage::RenderAttachment;
		swapChainDesc.format = swapChainFormat;
		swapChainDesc.presentMode = PresentMode::Fifo;
		SwapChain swapChain = m_device.createSwapChain(m_surface, swapChainDesc);
		const char* shaderSource = R"(
@vertex
fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4<f32> {
	var p = vec2f(0.0, 0.0);
	if (in_vertex_index == 0u) {
		p = vec2f(-0.5, -0.5);
	} else if (in_vertex_index == 1u) {
		p = vec2f(0.5, -0.5);
	} else {
		p = vec2f(0.0, 0.5);
	}
	return vec4f(p, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
    return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";

		ShaderModuleDescriptor shaderDesc;
#ifdef WEBGPU_BACKEND_WGPU
		shaderDesc.hintCount = 0;
		shaderDesc.hints = nullptr;
#endif

		// Use the extension mechanism to load a WGSL shader source code
		ShaderModuleWGSLDescriptor shaderCodeDesc;
		// Set the chained struct's header
		shaderCodeDesc.chain.next = nullptr;
		shaderCodeDesc.chain.sType = SType::ShaderModuleWGSLDescriptor;
		// Connect the chain
		shaderDesc.nextInChain = &shaderCodeDesc.chain;

		// Setup the actual payload of the shader code descriptor
		shaderCodeDesc.code = shaderSource;

		ShaderModule shaderModule = m_device.createShaderModule(shaderDesc);
		std::cout << "Shader module: " << shaderModule << std::endl;

		std::cout << "Creating render pipeline..." << std::endl;
		RenderPipelineDescriptor pipelineDesc;

		// Vertex fetch
		// (We don't use any input buffer so far)
		pipelineDesc.vertex.bufferCount = 0;
		pipelineDesc.vertex.buffers = nullptr;

		// Vertex shader
		pipelineDesc.vertex.module = shaderModule;
		pipelineDesc.vertex.entryPoint = "vs_main";
		pipelineDesc.vertex.constantCount = 0;
		pipelineDesc.vertex.constants = nullptr;

		// Primitive assembly and rasterization
		// Each sequence of 3 vertices is considered as a triangle
		pipelineDesc.primitive.topology = PrimitiveTopology::TriangleList;
		// We'll see later how to specify the order in which vertices should be
		// connected. When not specified, vertices are considered sequentially.
		pipelineDesc.primitive.stripIndexFormat = IndexFormat::Undefined;
		// The face orientation is defined by assuming that when looking
		// from the front of the face, its corner vertices are enumerated
		// in the counter-clockwise (CCW) order.
		pipelineDesc.primitive.frontFace = FrontFace::CCW;
		// But the face orientation does not matter much because we do not
		// cull (i.e. "hide") the faces pointing away from us (which is often
		// used for optimization).
		pipelineDesc.primitive.cullMode = CullMode::None;

		// Fragment shader
		FragmentState fragmentState;
		pipelineDesc.fragment = &fragmentState;
		fragmentState.module = shaderModule;
		fragmentState.entryPoint = "fs_main";
		fragmentState.constantCount = 0;
		fragmentState.constants = nullptr;

		// Configure blend state
		BlendState blendState;
		// Usual alpha blending for the color:
		blendState.color.srcFactor = BlendFactor::SrcAlpha;
		blendState.color.dstFactor = BlendFactor::OneMinusSrcAlpha;
		blendState.color.operation = BlendOperation::Add;
		// We leave the target alpha untouched:
		blendState.alpha.srcFactor = BlendFactor::Zero;
		blendState.alpha.dstFactor = BlendFactor::One;
		blendState.alpha.operation = BlendOperation::Add;

		ColorTargetState colorTarget;
		colorTarget.format = swapChainFormat;
		colorTarget.blend = &blendState;
		colorTarget.writeMask = ColorWriteMask::All; // We could write to only some of the color channels.

		// We have only one target because our render pass has only one output color
		// attachment.
		fragmentState.targetCount = 1;
		fragmentState.targets = &colorTarget;

		// Depth and stencil tests are not used here
		pipelineDesc.depthStencil = nullptr;

		// Multi-sampling
		// Samples per pixel
		pipelineDesc.multisample.count = 1;
		// Default value for the mask, meaning "all bits on"
		pipelineDesc.multisample.mask = ~0u;
		// Default value as well (irrelevant for count = 1 anyways)
		pipelineDesc.multisample.alphaToCoverageEnabled = false;

		// Pipeline layout
		pipelineDesc.layout = nullptr;

		RenderPipeline pipeline = m_device.createRenderPipeline(pipelineDesc);
		std::cout << "Render pipeline: " << pipeline << std::endl;
		bool pause = false;
		canvas.onWindowResize([&](WindowSize size) {
			if (size.width == 0 || size.height == 0) {
				pause = true;
				return;
			}

			pause = false;

			SwapChainDescriptor swapChainDesc;
			swapChainDesc.width = size.width;
			swapChainDesc.height = size.height;
			swapChainDesc.usage = TextureUsage::RenderAttachment;
			swapChainDesc.format = swapChainFormat;
			swapChainDesc.presentMode = PresentMode::Fifo;
			swapChain = m_device.createSwapChain(m_surface, swapChainDesc);
			});

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			if (pause) continue;

			TextureView nextTexture = swapChain.getCurrentTextureView();

			CommandEncoderDescriptor commandEncoderDesc;
			commandEncoderDesc.label = "Command Encoder";
			CommandEncoder encoder = m_device.createCommandEncoder(commandEncoderDesc);

			RenderPassDescriptor renderPassDesc;

			RenderPassColorAttachment renderPassColorAttachment;
			renderPassColorAttachment.view = nextTexture;
			renderPassColorAttachment.resolveTarget = nullptr;
			renderPassColorAttachment.loadOp = LoadOp::Clear;
			renderPassColorAttachment.storeOp = StoreOp::Store;
			renderPassColorAttachment.clearValue = Color{ 0.9, 0.1, 0.2, 1.0 };
			renderPassDesc.colorAttachmentCount = 1;
			renderPassDesc.colorAttachments = &renderPassColorAttachment;

			renderPassDesc.depthStencilAttachment = nullptr;
			renderPassDesc.timestampWriteCount = 0;
			renderPassDesc.timestampWrites = nullptr;
			RenderPassEncoder renderPass = encoder.beginRenderPass(renderPassDesc);

			renderPass.setPipeline(pipeline);
			renderPass.draw(3, 1, 0, 0);
			renderPass.end();
			renderPass.release();

			nextTexture.release();

			CommandBufferDescriptor cmdBufferDescriptor;
			cmdBufferDescriptor.label = "Command buffer";
			CommandBuffer command = encoder.finish(cmdBufferDescriptor);
			encoder.release();
			m_queue.submit(command);
			command.release();

			swapChain.present();
		}
		pipeline.release();
		shaderModule.release();
		swapChain.release();
		m_device.release();
		m_adapter.release();
		m_instance.release();
		m_surface.release();
		glfwDestroyWindow(window);
		glfwTerminate();
	}
};

WebGPURenderer::WebGPURenderer() {

}


void WebGPURenderer::setContainer(Canvas& canvas) {
	pimpl_ = std::make_shared<Impl>(canvas);
}

void WebGPURenderer::render() {

}