#include <nodes/materials/NodeMaterial.h>

using namespace mix;

std::shared_ptr<NodeMaterial> NodeMaterial::create() {
	return std::make_shared<NodeMaterial>();
}

std::shared_ptr<NodeMaterial> NodeMaterial::fromMaterial(Material* material) {

}

NodeMaterial::NodeMaterial() :ShaderMaterial() {

}

std::string NodeMaterial::type() const {
	return "NodeMaterial";
}