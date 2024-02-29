#pragma once

#include <string>

namespace mix {
	struct NodeFunctionInput{

		std::string type;
		std::string name;
		uint32_t count = 0;
		std::string qualifier;
		bool isConst = false;
	};
}