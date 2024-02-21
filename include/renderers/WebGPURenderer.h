#pragma once
#include <memory>

namespace mix {
	class Canvas;

	class WebGPURenderer {
	public:
		WebGPURenderer();

		void setContainer(Canvas& canvas);

		void render();

	private:
		struct Impl;
		std::shared_ptr<Impl> pimpl_;
	};
}
