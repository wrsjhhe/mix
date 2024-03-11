#include <nodes/core/NodeUniform.h>
#include <nodes/core/Node.h>
using namespace mix;

NodeUniform::NodeUniform(const std::string& name, const std::string& type, Node* node, bool needsUpdate) :
	name(name),type(type),node(node->getSelf()),needsUpdate(needsUpdate)
{

}

void* NodeUniform::value() {
	return node->value;
}

void NodeUniform::setValue(void* value) {
	node->value = value;
}

uint32_t NodeUniform::id() {
	return node->id;
}

UniformGroupNode* NodeUniform::groupNode() {
	return node->groupNode.get();
}