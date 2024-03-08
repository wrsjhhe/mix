#pragma once
#include <math/Plane.h>
#include <vector>
#include <materials/ShaderMaterial.h>
namespace mix {
	class LightsNode;
	class EnvironmentNode;
	class NodeMaterial : public ShaderMaterial{
	public:
		virtual std::string type() const override;

		bool forceSinglePass = false;

		bool fog = true;
		bool lights = true;
		bool normals = true;

		bool colorSpaced = true;

		LightsNode* lightsNode = nullptr;
		EnvironmentNode* envNode = nullptr;

		static std::shared_ptr<NodeMaterial> create();
		static std::shared_ptr<NodeMaterial> fromMaterial(Material* material);



	protected:
		NodeMaterial();
	};
}