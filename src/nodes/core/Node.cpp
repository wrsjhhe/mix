#include <nodes/core/Node.h>
#include <nodes/core/NodeUtils.h>
#include <nodes/core/NodeBuilder.h>
#include <iostream>
#include <format>

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


std::string Node::getNodeType(NodeBuilder* builder) {
	auto& nodeProperties = builder->getNodeProperties(this);
	if (nodeProperties.outputNode) {
		return nodeProperties.outputNode->getNodeType(builder);
	}

	return nodeType;
}

std::string Node::build(NodeBuilder* builder, const std::string& output) {
	return "";
}