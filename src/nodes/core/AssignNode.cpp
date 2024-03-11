#include <nodes/core/AssignNode.h>
#include <nodes/core/NodeBuilder.h>
#include <nodes/core/constants.h>
#include <utils/StringUtils.h>
#include <nodes/utils/SplitNode.h>

using namespace mix;

AssignNode::AssignNode(Node* targetNode, Node* sourceNode):
	targetNode(targetNode),sourceNode(sourceNode) 
{

}

std::string AssignNode::getNodeType(NodeBuilder* builder, const std::string& output) {
	return output != "void" ? targetNode->getNodeType(builder) : "void";
}

bool AssignNode::needsSplitAssign(NodeBuilder* builder) {
	if (builder->isAvailable("swizzleAssign") == false) {
		SplitNode* splitNode = dynamic_cast<SplitNode*>(targetNode);
		if (splitNode && splitNode->components.size() > 1) {
			auto targetLength = builder->getTypeLength(splitNode->node->getNodeType(builder));
			std::string str = utils::join(vectorComponents, "");
			str = utils::slice(str, 0, targetLength);

			return str != splitNode->components;
		}
	}

	return false;
}

