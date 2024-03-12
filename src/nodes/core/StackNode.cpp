#include <nodes/core/StackNode.h>

using namespace mix;

StackNode::StackNode(StackNode* parent):
	parent(parent)
{

}

std::string StackNode::getNodeType(NodeBuilder* builder) {
	return outputNode != nullptr ? outputNode->getNodeType(builder) : "void";
}

StackNode* StackNode::add(Node* node) {
	nodes.push_back(node);
	return this;
}