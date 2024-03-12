#pragma once

#include "nodes/core/NodeBuilder.h"

namespace mix {
	class Object3D;
	class Renderer;
	class Scene;

	class WGSLNodeBuilder : public NodeBuilder {
	public:
		struct UniformGroups {

		};

		struct Builtins
		{

		};

	public:
		UniformGroups uniformGroups;
		Builtins builtins;

		WGSLNodeBuilder(Object3D* object,Renderer* renderer, Scene* scene = nullptr);
		virtual ~WGSLNodeBuilder();

		virtual void buildCode() override;
	};
}