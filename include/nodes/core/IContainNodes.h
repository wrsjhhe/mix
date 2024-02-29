#pragma once

#include <unordered_map>
#include <string>

namespace mix {
	class Node;
	class IContainNodes {
	public:
		virtual std::unordered_map<std::string, Node*>& getChildren() {
			return children;
		};

	protected:
		std::unordered_map<std::string, Node*> children;
	};
}