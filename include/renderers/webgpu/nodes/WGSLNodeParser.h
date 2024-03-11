#pragma once

#include <nodes/core/NodeParser.h>
#include <renderers/webgpu/nodes/WGSLNodeFunction.h>
namespace mix {
	class WGSLNodeParser : public NodeParser {
	public:
		virtual std::shared_ptr<NodeFunction> parseFunction(const std::string& source) {
			return std::make_shared<WGSLNodeFunction>(source);
		}

		virtual ~WGSLNodeParser(){}
	};
}