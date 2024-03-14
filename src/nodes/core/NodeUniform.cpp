#include <nodes/core/NodeUniform.h>
#include <nodes/core/UniformNode.h>
using namespace mix;

NodeUniform::NodeUniform(const std::string& name, const std::string& type, UniformNode* node, bool needsUpdate) :
	name(name),type(type),node(node),needsUpdate(needsUpdate)
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