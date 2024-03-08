#include <renderers/common/nodes/NodeBuilderState.h>

using namespace mix;

NodeBuilderState::NodeBuilderState(const std::string& vertexShader, const std::string& fragmentShader, const std::string& computeShader,
	const std::vector<NodeAttribute*>& nodeAttributes, Bindings* bindings,
	const std::vector<Node*>& updateNodes, const std::vector<Node*>& updateBeforeNodes) :
	vertexShader(vertexShader),fragmentShader(fragmentShader), computeShader(computeShader),
	nodeAttributes(nodeAttributes), bindings(bindings),
	updateNodes(updateNodes), updateBeforeNodes(updateBeforeNodes)
{

}