#include <nodes/core/Node.h>

static unsigned int _nodeId = 0;

using namespace mix;

Node::Node(int nodeType):EventDispatcher(), id(_nodeId++),nodeType(nodeType) {

}