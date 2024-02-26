#pragma once
#include <renderers/common/Backend.h>

namespace mix {
	class WebGPURenderer;

	class WebGPUBackend : public Backend{
	public:
		WebGPUBackend(const Parameters& parameters = { });

		void init(std::shared_ptr<WebGPURenderer>& renderer);
	private:
		struct Impl;
		std::shared_ptr<Impl> pimpl_;
	};
}