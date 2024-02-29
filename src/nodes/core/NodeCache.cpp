#include <nodes/core/NodeCache.h>

using namespace mix;

static unsigned int _id = 0;

NodeCache::NodeCache():id(_id++){

}

NodeData* NodeCache::getNodeData(Node* node) {
	return nodesData[node].get();
}

void NodeCache::setNodeData(Node* node, std::shared_ptr<NodeData>& data) {
	nodesData[node] = data;
}