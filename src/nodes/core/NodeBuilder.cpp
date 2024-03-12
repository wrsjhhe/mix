#include <nodes/core/NodeBuilder.h>
#include <core/Object3D.h>
#include <scenes/Scene.h>
#include <materials/Material.h>
#include <nodes/core/NodeParser.h>
#include <nodes/core/Node.h>
#include <nodes/core/NodeCache.h>
#include <nodes/core/StackNode.h>
#include <nodes/shadernode/ShaderNode.h>
#include <renderers/common/Renderer.h>
#include <nodes/materials/NodeMaterial.h>
#include <nodes/core/constants.h>
#include <unordered_map>
#include <string>
#include <regex>
#include <format>

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

	cache = std::make_shared<NodeCache>();
	globalCache = cache.get();
}
NodeBuilder::~NodeBuilder(){}

NodeStageDataProperties& NodeBuilder::getNodeProperties(Node* node, const std::string& shaderStage) {
	NodeStageData& nodeData = getDataFromNode(node, shaderStage);
	return nodeData.properties;
}

NodeStageData& NodeBuilder::getDataFromNode(Node* node, std::string shaderStage, NodeCache* cache) {
	if (shaderStage.empty()) {
		shaderStage = this->shaderStage;
	}

	cache = cache == nullptr ? (node->isGlobal() ? globalCache : cache) : cache;
	if (cache == nullptr) {
		throw std::exception("cache is null");
	}
	NodeData* nodeData = cache->getNodeData(node);
	if (nodeData == nullptr) {
		cache->setNodeData(node, std::make_shared<NodeData>());
	}

	if (nodeData->find(shaderStage) == nodeData->end()) (*nodeData)[shaderStage] = {};

	return (*nodeData)[shaderStage];
}

NodeBuilder* NodeBuilder::build(bool convertMaterial) {
	if (convertMaterial) {
		if (material != nullptr) {
			NodeMaterial::fromMaterial(material)->build(this);
		}
		else {
			//addFlow("compute", object);
		}
	}

	for (auto& buildStage : defaultBuildStages) {
		setBuildStage(buildStage);

		if (context.vertex) {
			flowNodeFromShaderStage("vertex", context.vertex);
		}

		for (auto& shaderStage : shaderStages) {
			setShaderStage(shaderStage);

			auto flowNodes = this->flowNodes.value[shaderStage];

			for (auto& node : flowNodes) {
				if (buildStage == "generate") {
					//flowNode(node);
				}
				else {
					node->build(this);
				}
			}
		}
	}

	setBuildStage(nullptr);
	setShaderStage(nullptr);

	// stage 4: build code for a specific output

	buildCode();
	buildUpdateNodes();

	return this;
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

StackNode* NodeBuilder::addStack() {
	std::shared_ptr<StackNode> _stack = std::make_shared<StackNode>(stack);
	stacks.emplace_back(_stack);
	stack = _stack.get();
	setCurrentStack(stack);
	return stack;
}

StackNode* NodeBuilder::removeStack() {
	StackNode* lastStack = stack;
	stack = lastStack->parent;

	auto back = stacks.back();
	stacks.pop_back();
	setCurrentStack(back.get());

	return lastStack;
}

void NodeBuilder::setBuildStage(const std::string& buildStage) {
	this->buildStage = buildStage;
}
const std::string& NodeBuilder::getBuildStage() {
	return buildStage;
}

void NodeBuilder::setShaderStage(const std::string& shaderStage) {
	this->shaderStage = shaderStage;
}
const std::string& NodeBuilder::getShaderStage() {
	return shaderStage;
}

NodeBuilder::FlowData NodeBuilder::flowNodeFromShaderStage(const std::string& shaderStage, Node* node, const std::string& output, const std::string& propertyName) {
	const std::string& previousShaderStage = shaderStage;
	setShaderStage(shaderStage);

	FlowData flowData = flowChildNode(node, output);

	if (propertyName != utils::emptyString()) {
		flowData.code += std::format("{} = {};\n", tab + propertyName, flowData.result);
	}

	flowCode.value[shaderStage] = flowCode.value[shaderStage] + flowData.code;

	setShaderStage(previousShaderStage);

	return flowData;
}

NodeBuilder::FlowData NodeBuilder::flowChildNode(Node* node, const std::string& output) {
	FlowData previousFlow = flow;

	FlowData flow;
	this->flow = flow;

	flow.result = node->build(this, output);

	this->flow = previousFlow;

	return flow;
}

void NodeBuilder::buildUpdateNodes() {
	for (auto& node : nodes) {

		auto updateType = node->getUpdateType();
		auto updateBeforeType = node->getUpdateBeforeType();

		if (updateType != NodeUpdateType::NONE) {
			updateNodes.emplace_back(node->getSelf());
		}

		if (updateBeforeType != NodeUpdateType::NONE) {
			updateBeforeNodes.emplace_back(node);
		}
	}
}