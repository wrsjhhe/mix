#include <renderers/webgpu/nodes/WGSLNodeBuilder.h>
#include <renderers/webgpu/nodes/WGSLNodeParser.h>
using namespace mix;

WGSLNodeBuilder::WGSLNodeBuilder(Object3D* object, Renderer* renderer, Scene* scene) :
	NodeBuilder(object,renderer, std::shared_ptr<NodeParser>(), scene)
{

}