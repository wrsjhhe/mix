#pragma once
#include <string>
#include <vector>

namespace mix {
	class NodeAttribute;
	class Bindings;
	class Node;

	class NodeBuilderState {
	public:
		std::string vertexShader;
		std::string fragmentShader;
		std::string computeShader;
		std::vector<NodeAttribute*> nodeAttributes;
		Bindings* bindings;
		std::vector<Node*> updateNodes;
		std::vector<Node*> updateBeforeNodes;

		uint32_t usedTime = 0;

		NodeBuilderState(const std::string& vertexShader, const std::string& fragmentShader, const std::string& computeShader,
			const std::vector<NodeAttribute*>& nodeAttributes, Bindings* bindings,
			const std::vector<Node*>& updateNodes, const std::vector<Node*>& updateBeforeNodes);

		
	};
}