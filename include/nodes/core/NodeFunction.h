#pragma once

#include <string>
#include <vector>
#include <nodes/core/NodeFunctionInput.h>

namespace mix {
	class NodeFunction {
	public:

		std::string type;
		std::vector<NodeFunctionInput> inputs;
		std::string name;
		uint32_t presicion;

		virtual std::string getCode() = 0;

		static bool isNodeFunction;

		virtual ~NodeFunction(){}
	};

	bool NodeFunction::isNodeFunction = true;
}