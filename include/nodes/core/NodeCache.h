#pragma once

#include <unordered_map>
#include <memory>
#include <stdint.h>

namespace mix {
	struct NodeData {

	};

	class Node;
	class NodeCache {
	public:
		NodeCache();

		NodeData* getNodeData(Node* node);

		void setNodeData(Node* node, std::shared_ptr<NodeData>& data);

	private:
		uint32_t id;
		std::unordered_map<Node*, std::shared_ptr<NodeData>> nodesData;
	};
}