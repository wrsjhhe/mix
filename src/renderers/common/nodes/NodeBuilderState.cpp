#include <renderers/common/nodes/NodeBuilderState.h>
#include <renderers/common/nodes/NodeUniformsGroup.h>

using namespace mix;

NodeBuilderState::NodeBuilderState(const std::string& vertexShader, const std::string& fragmentShader, const std::string& computeShader,
	const std::vector<NodeAttribute*>& nodeAttributes, const std::vector<std::shared_ptr<NodeUniformsGroup>>& bindings,
	const std::vector<Node*>& updateNodes, const std::vector<Node*>& updateBeforeNodes) :
	vertexShader(vertexShader),fragmentShader(fragmentShader), computeShader(computeShader),
	nodeAttributes(nodeAttributes), bindings(bindings),
	updateNodes(updateNodes), updateBeforeNodes(updateBeforeNodes)
{
}

std::vector<std::shared_ptr<NodeUniformsGroup>> NodeBuilderState::createBindings() {
	std::vector<std::shared_ptr<NodeUniformsGroup>> bindingsArray;

	for (auto& instanceBinding : bindings) {
		std::shared_ptr<NodeUniformsGroup> binding = instanceBinding;
		if (!instanceBinding->shared()) {
			binding = std::make_shared<NodeUniformsGroup>();
			*binding = *instanceBinding;
		}

		bindingsArray.emplace_back(binding);
	}
	return bindingsArray;
}