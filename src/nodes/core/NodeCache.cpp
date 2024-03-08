#include <nodes/core/NodeCache.h>

using namespace mix;

static unsigned int _id = 0;

NodeCache::NodeCache():id(_id++){

}

NodeData* NodeCache::getNodeData(Node* node) {
	auto iter = nodesData.find(node);
	if (iter != nodesData.end()) {
		return iter->second.get();
	}
	return nullptr;
}

void NodeCache::setNodeData(Node* node,const std::shared_ptr<NodeData>& data) {
	nodesData[node] = data;
}