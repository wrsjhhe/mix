#include <renderers/common/nodes/Nodes.h>
#include <renderers/common/Renderer.h>
#include <renderers/common/Backend.h>
#include <nodes/core/NodeFrame.h>

using namespace mix;

Nodes::Nodes(const std::shared_ptr<Renderer>renderer, const std::shared_ptr<Backend> backend) 
	: renderer(renderer), backend(backend),
		nodeFrame(new NodeFrame()){

}
