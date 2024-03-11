#pragma once

#include <string>
#include <array>

namespace mix {
	enum class NodeShaderStage
	{
		VERTEX,
		FRAGMENT
	};

	enum class NodeUpdateType
	{
		NONE,
		FRAME,
		RENDER,
		OBJECT
	};

	enum class NodeType
	{
		BOOLEAN,
		INTEGER,
		FLOAT,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		MATRIX2,
		MATRIX3,
		MATRIX4,
	};

	extern std::array<std::string,2> defaultShaderStages;
	extern std::array<std::string, 3> defaultBuildStages;
	extern std::array<std::string, 3> shaderStages;
	extern std::array<std::string, 4> vectorComponents;
}

