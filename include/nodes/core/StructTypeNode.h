#pragma once

#include <nodes/core/Node.h>
#include <vector>
#include <string>

namespace mix {
	class StructTypeNode : public Node{
	public:
		std::vector<std::string> types;

		StructTypeNode(const std::vector<std::string>& types):
			Node(), types(types)
		{
			static std::function<std::shared_ptr<StructTypeNode>()> creator = []() {
				return std::shared_ptr<StructTypeNode>();
			};
			Node::addNodeClass("StructTypeNode", creator);
		}

		virtual std::string type() override {
			return "StructTypeNode";
		}

		const std::vector<std::string>& ()  {
			return types;
		}
	};
}