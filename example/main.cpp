#include <canvas/Canvas.h>
#include <renderers/WebGPURenderer.h>
#include <scenes/Scene.h>
#include <cameras/PerspectiveCamera.h>

//using namespace wgpu;
using namespace mix;

int main(int, char**) {

	Canvas canvas("BoxGeometry", { });
	WebGPURenderer renderer;

	auto scene = Scene::create();
	auto camera = PerspectiveCamera::create(60, 800/600, 0.1, 100);

	renderer.render(*scene,*camera);

	return 0;
}
