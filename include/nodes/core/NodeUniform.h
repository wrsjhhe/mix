#pragma once

#include <string>

namespace mix {
	class UniformNode;
	class UniformGroupNode;

	class NodeUniform {
	public:
		std::string name;
		std::string type;
		UniformNode* node = nullptr;
		bool needsUpdate = false;
		
		NodeUniform(const std::string& name,const std::string& type, UniformNode* node, bool needsUpdate = false);

		void* value();

		void setValue(void* value);

		UniformGroupNode* groupNode();

		uint32_t id();

	};
}