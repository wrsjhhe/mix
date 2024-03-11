#pragma once

#include <nodes/core/TempNode.h>

namespace mix {
	class NodeBuilder;

	class AssignNode {
	public:
		Node* targetNode;
		Node* sourceNode;

		AssignNode(Node* targetNode,Node* sourceNode);

		bool hasDependencies() { return false; }

		std::string getNodeType(NodeBuilder* builder,const std::string& output);

		bool needsSplitAssign(NodeBuilder* builder);
	};
}