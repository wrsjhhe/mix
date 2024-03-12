#pragma once

#include <nodes/core/constants.h>
#include <math/MathUtils.h>
#include <core/EventDispatcher.h>
#include <nodes/core/IContainNodes.h>
#include <functional>
#include <string>
#include <utils/StringUtils.h>

namespace mix {
	typedef std::function<void(Node*)> NodeTraverseCallback;

	class NodeBuilder;
	class UniformGroupNode;

	class Node : public EventDispatcher, public IContainNodes {
	public:
		unsigned int id;

		std::string nodeType = "";
		NodeUpdateType updateType = NodeUpdateType::NONE;
		NodeUpdateType updateBeforeType = NodeUpdateType::NONE;

		std::string uuid = math::generateUUID();

		void* value;

		std::shared_ptr<UniformGroupNode> groupNode;

		virtual std::string type() = 0;

		Node* getSelf() { return this; }

		explicit Node(const std::string& nodeType = "");

		virtual ~Node();

		virtual bool isGlobal() { return false; }

		virtual void dispose() { dispatchEvent("dispose"); }

		virtual void traverse(NodeTraverseCallback callback);

		virtual std::string getCacheKey();

		virtual std::string getHash( /*builder*/) { return uuid; }

		NodeUpdateType getUpdateType() { return updateType; }

		NodeUpdateType getUpdateBeforeType() { return updateBeforeType; }

		virtual std::string getNodeType(NodeBuilder* builder);

		virtual std::string build(NodeBuilder* builder,const std::string& output = utils::emptyString());
	};

	class NodeCreator {
	public:
		template<typename T, typename ...Args, typename = typename std::enable_if<std::is_base_of_v<Node, T>, void>::type>
		static std::shared_ptr<T> creatByType(Args... args) {
			return std::make_shared<T>(...args);
		}
	};
}