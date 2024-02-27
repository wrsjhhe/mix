#pragma once

#include <nodes/core/constants.h>
#include <string>
#include <math/MathUtils.h>
#include <core/EventDispatcher.h>

namespace mix {
	class Node : public EventDispatcher {
	public:
		unsigned int id;

		int nodeType = 0;
		NodeUpdateType updateType = NodeUpdateType::NONE;
		NodeUpdateType updateBeforeType = NodeUpdateType::NONE;

		std::string uuid = math::generateUUID();

		bool isNode = true;

		explicit Node(int nodeType = 0);
	};
}