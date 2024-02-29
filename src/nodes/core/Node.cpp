#include <nodes/core/Node.h>
#include <nodes/core/NodeUtils.h>

static unsigned int _nodeId = 0;

using namespace mix;

Node::Node(const std::string& nodeType):EventDispatcher(), id(_nodeId++),nodeType(nodeType) {

}

Node::~Node(){}


std::string Node::getCacheKey() {
	return NodeUtils::getCacheKey(this);
}

void Node::traverse(NodeTraverseCallback callback) {
	callback(this);

	for (auto childNode : this->getChildren()) {
		childNode.second->traverse(callback);
	}
}


NodeUpdateType Node::getNodeType() {

}