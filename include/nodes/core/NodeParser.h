#pragma once

#include <memory>
#include <string>

namespace mix {
	class NodeFunction;
	class NodeParser {
	public:
		virtual ~NodeParser() {}
		virtual std::shared_ptr<NodeFunction> parseFunction(const std::string& source) = 0;
	};
}