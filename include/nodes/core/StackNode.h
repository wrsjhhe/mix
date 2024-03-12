#pragma once

#include <nodes/core/Node.h>

#include <vector>

namespace mix {
	class ConNode;
	class NodeBuilder;
	class StackNode : public Node {
	public:
		std::vector<Node*> nodes;
		Node* outputNode;
		StackNode* parent;

		StackNode(StackNode* parent = nullptr);

		virtual std::string type() override { return "StackNode"; }

		virtual std::string getNodeType(NodeBuilder* builder) override;

		StackNode* add(Node* node);


	private:
		std::shared_ptr<ConNode> _currentCond;
	};
}