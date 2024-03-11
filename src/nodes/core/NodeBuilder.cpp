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
#include <regex>

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
			//addFlow("compute", object);
		}

	}
}

uint32_t NodeBuilder::getTypeLength(const std::string& type) {
	auto vecType = getVectorType(type);

	static std::regex regExp(R"(vec([2-4]))", std::regex_constants::icase);
	auto strIter = std::sregex_iterator(type.begin(), type.end(), regExp);
	if (strIter->size() == 2) {
		auto iter = strIter->begin();
		++iter;
		return std::atoi(iter->str().c_str());
	}
	
	if (vecType == "float" || vecType == "bool" || vecType == "int" || vecType == "uint") return 1;

	if (type.find("mat2") != std::string::npos) return 4;
	if (type.find("mat3") != std::string::npos) return 9;
	if (type.find("mat4") != std::string::npos) return 16;

	return 0;
}

std::string NodeBuilder::getVectorType(std::string type) {
	if (type == "color") return "vec3";
	if (type == "texture" || type == "cubeTexture" || type == "storageTexture") return "vec4";

	return type;
}