#pragma once
#include <math/Plane.h>
#include <vector>
#include <materials/ShaderMaterial.h>
namespace mix {
	class LightsNode;
	class EnvironmentNode;
	class NodeBuilder;

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

		virtual void copyMaterialProperties(Material* material);

		void build(NodeBuilder* builder);

		static std::shared_ptr<NodeMaterial> create(const std::unordered_map<std::string, MaterialValue>& values = {});
		static NodeMaterial* fromMaterial(Material* material);
		static NodeMaterial* createNodeMaterialFromType(const std::string& type);


	protected:
		NodeMaterial();
	};
	struct NodeMaterialCreator
	{
		virtual NodeMaterial* create() { nullptr; };
	};
}