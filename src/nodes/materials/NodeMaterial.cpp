#include <nodes/materials/NodeMaterial.h>
#include <utils/StringUtils.h>
#include <iostream>
#include <format>
using namespace mix;

static std::unordered_map<std::string, NodeMaterialCreator> NodeMaterials;

std::shared_ptr<NodeMaterial> NodeMaterial::create(const std::unordered_map<std::string, MaterialValue>& values) {

	auto m = std::shared_ptr<NodeMaterial>(new NodeMaterial());
	m->setValues(values);

	return m;
}

NodeMaterial* NodeMaterial::fromMaterial(Material* material) {
	NodeMaterial* nodeMaterial = dynamic_cast<NodeMaterial*>(material);
	if (nodeMaterial != nullptr) {
		return nodeMaterial;
	}

	std::string type = material->type();
	utils::replaceAll(type, "Material", "NodeMaterial");

	nodeMaterial = createNodeMaterialFromType(type);
	if (nodeMaterial == nullptr) {
		std::cerr << std::format("NodeMaterial: Material {} is not compatible.", material->type()) << std::endl;
	}

	nodeMaterial->copyMaterialProperties(material);

	return nodeMaterial;
}

NodeMaterial* NodeMaterial::createNodeMaterialFromType(const std::string& type) {
	auto& creator = NodeMaterials.find(type)->second;
	return creator.create();
}

void NodeMaterial::copyMaterialProperties(Material* material) {

}

void NodeMaterial::build(NodeBuilder* builder) {

}

NodeMaterial::NodeMaterial() :ShaderMaterial() {

}

std::string NodeMaterial::type() const {
	return "NodeMaterial";
}

