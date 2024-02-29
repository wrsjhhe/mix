#pragma once

#include <string>
#include <nodes/core/NodeFunction.h>

namespace mix {
	class WGSLNodeFunction : public NodeFunction {
	public:
		std::string inputsCode;
		std::string blockCode;

		WGSLNodeFunction(const std::string& source);

		std::string getCode();
	};
}