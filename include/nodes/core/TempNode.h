#pragma once 

#include <nodes/core/Node.h>
#include <string>

namespace mix {
	class NodeBuilder;

	class TempNode : public Node {
	public :
		TempNode(const std::string& type);

		bool isTempNode = true;

		bool hasDependencies(NodeBuilder* builder);

		virtual void build(NodeBuilder* builder, void* output = nullptr) override;
	};
}