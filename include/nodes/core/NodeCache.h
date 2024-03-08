#pragma once

#include <unordered_map>
#include <memory>
#include <unordered_map>
#include <string>

namespace mix {

	class Node;
	struct NodeStageDataProperties
	{
		Node* outputNode = nullptr;
	};

	struct NodeStageData
	{
		NodeStageDataProperties properties;
		uint32_t usageCount = 1;
	};

	typedef std::unordered_map<std::string, NodeStageData> NodeData;


	class NodeCache {
	public:
		NodeCache();

		NodeData* getNodeData(Node* node);

		void setNodeData(Node* node,const std::shared_ptr<NodeData>& data);

	private:
		uint32_t id;
		std::unordered_map<Node*, std::shared_ptr<NodeData>> nodesData;
	};
}