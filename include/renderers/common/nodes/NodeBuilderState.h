#pragma once
#include <string>
#include <vector>
#include <memory>

namespace mix {
	class NodeAttribute;
	class NodeUniformsGroup;
	class Node;

	class NodeBuilderState {
	public:
		std::string vertexShader;
		std::string fragmentShader;
		std::string computeShader;
		std::vector<NodeAttribute*> nodeAttributes;
		std::vector<std::shared_ptr<NodeUniformsGroup>> bindings;
		std::vector<Node*> updateNodes;
		std::vector<Node*> updateBeforeNodes;

		uint32_t usedTimes = 0;

		NodeBuilderState(const std::string& vertexShader, const std::string& fragmentShader, const std::string& computeShader,
			const std::vector<NodeAttribute*>& nodeAttributes,const std::vector<std::shared_ptr<NodeUniformsGroup>>& bindings,
			const std::vector<Node*>& updateNodes, const std::vector<Node*>& updateBeforeNodes);

		std::vector<std::shared_ptr<NodeUniformsGroup>> createBindings();
	};
}