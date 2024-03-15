#pragma once

#include <renderers/common/UniformBuffer.h>

namespace mix {
	class UniformDescripter;
	class UniformsGroup : public UniformBuffer {
	public:

		std::vector<std::shared_ptr<UniformDescripter>> uniforms;
		UniformsGroup() = default;
		UniformsGroup(const std::string& name);
		virtual ~UniformsGroup();

		UniformsGroup* addUniform(const std::shared_ptr<UniformDescripter>& uniform);

		UniformsGroup* removeUniform(const std::shared_ptr<UniformDescripter>& uniform);

		const std::vector<float>& buffer();

		virtual uint32_t byteLength() const override;
	};
}