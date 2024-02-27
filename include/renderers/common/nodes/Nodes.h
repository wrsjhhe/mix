#pragma once
#include <memory>

namespace mix {
	class Renderer;
	class Backend;
	class NodeFrame;

	class Nodes {
	public:
		Nodes() = delete;
		Nodes(Renderer* renderer, Backend* backend);


		std::shared_ptr<NodeFrame> nodeFrame;
	private:
		Renderer* renderer;
		Backend* backend;

	};
}