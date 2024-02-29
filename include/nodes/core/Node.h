#pragma once

#include <nodes/core/constants.h>
#include <string>
#include <math/MathUtils.h>
#include <core/EventDispatcher.h>
#include <nodes/core/IContainNodes.h>
#include <functional>

namespace mix {
	typedef std::function<void(Node*)> NodeTraverseCallback;

	class Node : public EventDispatcher, public IContainNodes {
	public:
		unsigned int id;

		std::string nodeType = "";
		NodeUpdateType updateType = NodeUpdateType::NONE;
		NodeUpdateType updateBeforeType = NodeUpdateType::NONE;

		std::string uuid = math::generateUUID();

		bool isNode = true;

		explicit Node(const std::string& nodeType = "");

		virtual ~Node();

		virtual bool isGlobal() { return false; }

		virtual void dispose() { dispatchEvent("dispose"); }

		virtual void traverse(NodeTraverseCallback callback);

		virtual std::string getCacheKey();

		virtual std::string getHash( /*builder*/) { return uuid; }

		NodeUpdateType getUpdateType() { return updateType; }

		NodeUpdateType getUpdateBeforeType() { return updateBeforeType; }

		NodeUpdateType getNodeType();
	};
}