#pragma once

#include <renderers/common/UniformBuffer.h>

namespace mix {
	class Uniform;

	class UniformsGroup : public UniformBuffer {
	public:

		std::vector<std::shared_ptr<Uniform>> uniforms;

		UniformsGroup(const std::string& name);
		~UniformsGroup();

		UniformsGroup* addUniform(const std::shared_ptr<Uniform>& uniform);

		UniformsGroup* removeUniform(const std::shared_ptr<Uniform>& uniform);

		std::vector<float> buffer();
	};
}