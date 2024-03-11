#pragma once

#include <string>

namespace mix {
	class Node;
	class UniformGroupNode;

	class NodeUniform {
	public:
		std::string name;
		std::string type;
		Node* node = nullptr;
		bool needsUpdate = false;
		
		NodeUniform(const std::string& name,const std::string& type, Node* node, bool needsUpdate = false);

		void* value();

		void setValue(void* value);

		UniformGroupNode* groupNode();

		uint32_t id();

	};
}