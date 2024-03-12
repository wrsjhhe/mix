#include <nodes/core/NodeBuilder.h>
#include <nodes/core/StackNode.h>
#include <nodes/materials/NodeMaterial.h>
#include <nodes/accessors/ModelViewProjectionNode.h>
#include <utils/StringUtils.h>
#include <core/Object3D.h>
#include <core/BufferGeometry.h>
#include <objects/SkinnedMesh.h>
#include <iostream>
#include <format>
using namespace mix;

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
	return nullptr;
}

void NodeMaterial::copyMaterialProperties(Material* material) {

}

void NodeMaterial::build(NodeBuilder* builder) {

}

void NodeMaterial::setup(NodeBuilder* builder) {
	// < VERTEX STAGE >
	builder->addStack();
	if (vertexNode != nullptr) {
		builder->stack->outputNode = vertexNode;
	}
	else {
		builder->stack->outputNode = setupPosition(builder);
	}
}

Node* NodeMaterial::setupPosition(NodeBuilder* builder) {
	Object3D* object = builder->object;
	BufferGeometry* geometry = object->geometry();

	builder->addStack();

	// Vertex
	const std::unordered_map<std::string, std::vector<std::shared_ptr<BufferAttribute>>>& morphAttributes = geometry->getMorphAttributes();
	if (morphAttributes.contains("position") || morphAttributes.contains("normal") || morphAttributes.contains("color")) {
		// morphReference(object).append();
	}

	if (dynamic_cast<SkinnedMesh*>(object) != nullptr) {
		// skinningReference( object ).append();
	}

	/*if ((object.instanceMatrix && object.instanceMatrix.isInstancedBufferAttribute == = true) && builder.isAvailable('instance') == = true) {
		instance(object).append();
	}*/

	//if (this.positionNode != = null) {
	//	positionLocal.assign(this.positionNode);
	//}

	builder->context.vertex = builder->removeStack();
}

NodeMaterial::NodeMaterial() :ShaderMaterial() {

}

std::string NodeMaterial::type() const {
	return "NodeMaterial";
}

