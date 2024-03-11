#include <nodes/display/ToneMappingNode.h>

using namespace mix;



ToneMappingNode::ToneMappingNode(ToneMapping toneMapping):TempNode("vec3"),toneMapping(toneMapping) {
	static std::function<std::shared_ptr<Node>()> creator = []() {
		return std::shared_ptr<ToneMappingNode>();
	};
	Node::addNodeClass("ToneMappingNode", creator);
}



