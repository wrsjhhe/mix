#include <canvas/Canvas.h>
#include <renderers/WebGPURenderer.h>

//using namespace wgpu;
using namespace mix;

int main(int, char**) {

	Canvas canvas("BoxGeometry", { });
	WebGPURenderer renderer;
	renderer.setContainer(canvas);

	return 0;
}
