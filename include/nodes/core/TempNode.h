#pragma once 

#include <nodes/core/Node.h>
#include <string>

namespace mix {
	class TempNode : public Node {
	public :
		TempNode(const std::string& type);

		bool isTempNode() { return true; };
	};
}