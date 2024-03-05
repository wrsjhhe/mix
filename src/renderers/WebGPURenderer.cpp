#include <renderers/WebGPURenderer.h>
#include <renderers/common/Renderer.h>
#include <renderers/WebGPUBackend.h>
#include <memory>

using namespace mix;

WebGPURenderer::WebGPURenderer(Renderer::Parameters parameters) : Renderer(std::make_shared<WebGPUBackend>(parameters), parameters) {

}