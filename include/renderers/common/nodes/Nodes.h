#pragma once
#include <memory>

namespace mix {
	class Renderer;
	class Backend;
	class NodeFrame;

	class Nodes {
	public:
		Nodes() = delete;
		Nodes(const std::shared_ptr<Renderer> renderer, const std::shared_ptr<Backend> backend);


		std::shared_ptr<NodeFrame> nodeFrame;
	private:
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Backend> backend;

	};
}