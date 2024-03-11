#include <renderers/webgpu/nodes/WGSLNodeBuilder.h>
#include <renderers/webgpu/nodes/WGSLNodeParser.h>
#include <core/Object3D.h>
#include <renderers/common/Renderer.h>
#include <scenes/Scene.h>

using namespace mix;

WGSLNodeBuilder::WGSLNodeBuilder(Object3D* object, Renderer* renderer, Scene* scene) :
	NodeBuilder(object,renderer, std::shared_ptr<NodeParser>(), scene)
{

}