#include <nodes/core/TempNode.h>
#include <nodes/core/NodeBuilder.h>

using namespace mix;

TempNode::TempNode(const std::string& type) :Node(type) {

}

bool TempNode::hasDependencies(NodeBuilder* builder) {
	return builder->getDataFromNode(this).usageCount > 1;
}

void TempNode::build(NodeBuilder* builder, void* output) {

}