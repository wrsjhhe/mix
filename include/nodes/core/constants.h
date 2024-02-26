#include <unordered_map>
#include <string>

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
