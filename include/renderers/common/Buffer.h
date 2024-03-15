#pragma once

#include <renderers/common/Binding.h>
#include <vector>

namespace mix {
	class Buffer : public Binding {
	public:
		uint32_t bytesPerElement = sizeof(float);

		std::vector<float> _buffer;
		Buffer() = default;
		explicit Buffer(const std::string& name, const std::vector<float>& buffer = std::vector<float>());
		virtual ~Buffer();

		virtual uint32_t byteLength() const;

		const std::vector<float>& buffer() const;

		bool update();
	};
}