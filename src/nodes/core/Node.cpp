#include <nodes/core/Node.h>

static unsigned int _nodeId = 0;

using namespace mix;

Node::Node(const std::string& nodeType):EventDispatcher(), id(_nodeId++),nodeType(nodeType) {

}

bool Node::isGlobal() {
	return false;
}

std::string Node::getHash() {
	return uuid;
}