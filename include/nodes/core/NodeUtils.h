#pragma once

#include <string>
#include <nodes/core/IContainNodes.h>

namespace mix {
	class Node;

	class NodeUtils {
	public:
		static std::string getCacheKey(IContainNodes* object);

		static std::string getValueType(const std::string& value);
	};
}