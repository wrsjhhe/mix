#include <renderers/common/nodes/Nodes.h>
#include <renderers/common/Renderer.h>
#include <renderers/common/Backend.h>
#include <nodes/core/NodeFrame.h>

using namespace mix;

Nodes::Nodes(Renderer* renderer, Backend* backend) 
	: renderer(renderer), backend(backend),
		nodeFrame(std::make_shared<NodeFrame>()){

}
