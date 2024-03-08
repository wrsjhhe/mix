#pragma once

#include <string>

namespace mix {
	class Node;
	struct NodeAttribute
	{
		NodeAttribute(const std::string& name,const std::string type, Node* node):
		name(name),type(type),node(node)
		{

		}
		std::string name;
		std::string type;
		Node* node = nullptr;
	};
}