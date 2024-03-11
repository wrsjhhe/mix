#include <nodes/core/NodeBuilder.h>
#include <core/Object3D.h>
#include <scenes/Scene.h>
#include <materials/Material.h>
#include <nodes/core/NodeParser.h>
#include <nodes/core/Node.h>
#include <nodes/core/NodeCache.h>
#include <renderers/common/Renderer.h>
#include <nodes/materials/NodeMaterial.h>
#include <unordered_map>
#include <string>

using namespace mix;

std::unordered_map<uint32_t, std::string> typeFromLength = { 
	{2, "vec2"},
	{3, "vec3"},
	{4, "vec4"},
	{9, "mat3"},
	{16, "mat4"},
};


NodeBuilder::NodeBuilder(Object3D* object, Renderer* renderer, std::shared_ptr<NodeParser> parser, Scene* scene, Material* material):
	object(object), renderer(renderer), parser(parser), scene(scene), material(material)
{
	if (this->material == nullptr) {
		this->material = object->material();
	}

	context.material = material;
}

NodeStageDataProperties& NodeBuilder::getNodeProperties(Node* node, const std::string& shaderStage) {
	NodeStageData& nodeData = getDataFromNode(node, shaderStage);
	return nodeData.properties;
}

NodeStageData& NodeBuilder::getDataFromNode(Node* node, std::string shaderStage, NodeCache* cache) {
	if (shaderStage.empty()) {
		shaderStage = this->shaderStage;
	}

	cache = cache == nullptr ? (node->isGlobal() ? globalCache : cache) : cache;
	NodeData* nodeData = cache->getNodeData(node);
	if (nodeData == nullptr) {
		cache->setNodeData(node, std::make_shared<NodeData>());
	}

	if (nodeData->find(shaderStage) == nodeData->end()) (*nodeData)[shaderStage] = {};

	return (*nodeData)[shaderStage];
}

void NodeBuilder::build(bool convertMaterial) {
	if (convertMaterial) {
		if (material != nullptr) {
			NodeMaterial::fromMaterial(material)->build(this);
		}
		else {
			//addFlow('compute', object);
		}

	}
}