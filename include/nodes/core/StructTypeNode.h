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
		{}

		virtual std::string type() override {
			return "StructTypeNode";
		}

		const std::vector<std::string>& getMemberTypes()  {
			return types;
		}
	};
}