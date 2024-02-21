#pragma once

#include <memory>

namespace mix {
	class Backend;

	class Renderer {
	public:
		struct Parameters
		{

		};
		Renderer(const std::shared_ptr<Backend>& backend, const Parameters& parameters = {});

		void render();

	private:
		struct Impl;
		std::shared_ptr<Impl> pimpl_;
	};
}