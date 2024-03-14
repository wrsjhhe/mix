#include <canvas/Canvas.h>
#include <renderers/WebGPURenderer.h>
#include <scenes/Scene.h>
#include <cameras/PerspectiveCamera.h>

//using namespace wgpu;
using namespace mix;

int main(int, char**) {

	Canvas canvas("BoxGeometry", { });
	WebGPURenderer renderer;
	renderer.domElement = &canvas;

	auto scene = Scene::create();
	auto camera = PerspectiveCamera::create(60, 800/600, 0.1, 100);

	renderer.render(scene.get(), camera.get());

	return 0;
}
