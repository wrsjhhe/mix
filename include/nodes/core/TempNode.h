#pragma once 

#include <nodes/core/Node.h>
#include <string>
#include <utils/StringUtils.h>

namespace mix {
	class NodeBuilder;

	class TempNode : public Node {
	public :
		TempNode(const std::string& type);

		bool isTempNode = true;

		bool hasDependencies(NodeBuilder* builder);

		virtual std::string build(NodeBuilder* builder, const std::string& output = utils::emptyString()) override;
	};
}