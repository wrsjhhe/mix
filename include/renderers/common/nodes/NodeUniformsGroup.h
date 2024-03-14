#pragma once

#include <renderers/common/UniformsGroup.h>

namespace mix {
	class UniformGroupNode;
	class UniformNode;

	class NodeUniformsGroup : public UniformsGroup {
	public:
		uint32_t id;
		UniformGroupNode* groupNode = nullptr;

		NodeUniformsGroup(const std::string& name, UniformGroupNode* groupNode);
		virtual ~NodeUniformsGroup();
		bool shared();

		std::vector<UniformNode*> getNodes();
	};
}