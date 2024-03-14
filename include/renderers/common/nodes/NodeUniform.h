#pragma once

#include <renderers/common/Uniform.h>
#include <nodes/core/NodeUniform.h>

namespace mix {
	template<typename T, typename = typename std::enable_if<std::is_base_of_v<UniformDescripter, T>, void>::type>
	class TypedNodeUniform : public T {
	public:
		NodeUniform* nodeUniform;

		void* getValue() {
			return nodeUniform->value();
		}
	};

	typedef TypedNodeUniform<FloatUniform> FloatNodeUniform;
	typedef TypedNodeUniform<Vector2Uniform> Vector2NodeUniform;
	typedef TypedNodeUniform<Vector3Uniform> Vector3NodeUniform;
	typedef TypedNodeUniform<Vector4Uniform> Vector4NodeUniform;
	typedef TypedNodeUniform<ColorUniform> ColorNodeUniform;
	typedef TypedNodeUniform<Matrix3Uniform> Matrix3NodeUniform;
	typedef TypedNodeUniform<Matrix4Uniform> Matrix4NodeUniform;

}