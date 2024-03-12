#pragma once
#include <math/Plane.h>
#include <vector>
#include <materials/ShaderMaterial.h>
namespace mix {
	class LightsNode;
	class EnvironmentNode;
	class NodeBuilder;
	class Node;

	class NodeMaterial : public ShaderMaterial{
	public:
		virtual std::string type() const override;

		bool isShadowNodeMaterial = false;

		bool forceSinglePass = false;

		bool fog = true;
		bool lights = true;
		bool normals = true;

		bool colorSpaced = true;

		LightsNode* lightsNode = nullptr;
		EnvironmentNode* envNode = nullptr;

		Node* vertexNode;

		virtual void copyMaterialProperties(Material* material);

		void build(NodeBuilder* builder);

		void setup(NodeBuilder* builder);

		Node* setupPosition(NodeBuilder* builder);

		static std::shared_ptr<NodeMaterial> create(const std::unordered_map<std::string, MaterialValue>& values = {});
		static NodeMaterial* fromMaterial(Material* material);
		static NodeMaterial* createNodeMaterialFromType(const std::string& type);


	protected:
		NodeMaterial();
	};

	class NodeMaterialCreator {
	public:
		template<typename T, typename ...Args, typename = typename std::enable_if<std::is_base_of_v<NodeMaterial, T>, void>::type>
		static std::shared_ptr<T> creatByType(Args... args) {
			return std::make_shared<T>(...args);
		}
	};
}