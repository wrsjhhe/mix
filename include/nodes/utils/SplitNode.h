#pragma once

#include <nodes/core/Node.h>

namespace mix {
	class SplitNode : public Node {
	public:
		Node* node;
		std::string components;

		SplitNode(Node* node,const std::string& components = "x");
	};
}