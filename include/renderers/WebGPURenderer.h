#pragma once

#include <renderers/common/Renderer.h>

namespace mix {


	class WebGPURenderer : public Renderer {
	public:
		WebGPURenderer(Renderer::Parameters parameters = {});

	};
}
