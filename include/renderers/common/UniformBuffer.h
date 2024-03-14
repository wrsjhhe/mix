#pragma once

#include <renderers/common/Buffer.h>

namespace mix {
	class UniformBuffer : public Buffer {
	public:
		explicit UniformBuffer(const std::string& name, const std::vector<float>& buffer = std::vector<float>()):
			Buffer(name, buffer)
		{

		}
		virtual ~UniformBuffer(){}
	};
}