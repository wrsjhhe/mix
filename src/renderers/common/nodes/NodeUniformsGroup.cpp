#include <renderers/common/nodes/NodeUniformsGroup.h>
#include <nodes/core/UniformGroupNode.h>
#include <renderers/common/nodes/NodeUniform.h>
#include <assert.h>
#include <iostream>

using namespace mix;

static uint32_t _id = 0;

NodeUniformsGroup::NodeUniformsGroup(const std::string& name, UniformGroupNode* groupNode) :
	UniformsGroup(name), id(_id++),groupNode(groupNode)
{
}

NodeUniformsGroup::~NodeUniformsGroup() {

}

bool NodeUniformsGroup::shared(){
	return groupNode->shared;
}

std::vector<UniformNode*> NodeUniformsGroup::getNodes() {
	std::vector<UniformNode*> nodes;
	for (auto& uniform : uniforms) {
		auto typedUniform = static_cast<TypedNodeUniform<UniformDescripter>*>(uniform.get());
		auto& node = typedUniform->nodeUniform->node;
		if (!node) {
			std::cerr << "NodeUniformsGroup: Uniform has no node." << std::endl;
			assert(false);
		}
		nodes.emplace_back(node);
	}

	return nodes;
}