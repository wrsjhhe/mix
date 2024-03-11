#include <nodes/core/Node.h>
#include <nodes/core/NodeUtils.h>
#include <nodes/core/NodeBuilder.h>
#include <iostream>
#include <format>

static unsigned int _nodeId = 0;

using namespace mix;

static std::unordered_map<std::string,std::function<std::shared_ptr<Node>()>> NodeClasses;

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


const std::string& Node::getNodeType(NodeBuilder* builder) {
	auto& nodeProperties = builder->getNodeProperties(this);
	if (nodeProperties.outputNode) {
		return nodeProperties.outputNode->getNodeType(builder);
	}

	return nodeType;
}

void Node::build(NodeBuilder* builder, void* output) {

}


void Node::addNodeClass(const std::string& type, const std::function<std::shared_ptr<Node>()>& nodeClass) {
	if (NodeClasses.contains(type)) {
		return;
	}
	NodeClasses[type] = nodeClass;
}

std::shared_ptr<Node> Node::createNodeFromType(const std::string& type) {
	auto iter = NodeClasses.find(type);

	return iter->second();
}