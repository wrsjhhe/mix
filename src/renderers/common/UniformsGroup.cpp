#include <renderers/common/UniformsGroup.h>
#include <renderers/common/nodes/NodeUniform.h>
#include <renderers/common/Uniform.h>
#include <cmath>
#include <constants.h>

using namespace mix;

UniformsGroup::UniformsGroup(const std::string& name) :UniformBuffer(name) {

}

UniformsGroup::~UniformsGroup() {

}

UniformsGroup* UniformsGroup::addUniform(const std::shared_ptr<UniformDescripter>& uniform) {
	uniforms.emplace_back(uniform);
	return this;
}

UniformsGroup* UniformsGroup::removeUniform(const std::shared_ptr<UniformDescripter>& uniform) {
	uniforms.erase(std::remove(uniforms.begin(), uniforms.end(), uniform), uniforms.end());
}

const std::vector<float>& UniformsGroup::buffer() {
	std::vector<float>& buffer = _buffer;
	if (buffer.empty()) {
		buffer.resize(this->byteLength() / 4);
	}
	return buffer;
}

uint32_t UniformsGroup::byteLength() const {
	uint32_t offset = 0; // global buffer offset in bytes

	for (size_t i = 0; i < uniforms.size(); ++i) {
		auto& uniform = uniforms[i];

		uint32_t boundary = uniform->boundary;
		uint32_t itemSize = uniform->itemSize;

		// offset within a single chunk in bytes
		uint32_t chunkOffset = offset % GPU_CHUNK_BYTES;
		uint32_t remainingSizeInChunk = GPU_CHUNK_BYTES - chunkOffset;

		// conformance tests
		if (chunkOffset != 0 && (remainingSizeInChunk - boundary) < 0) {
			// check for chunk overflow
			offset += (GPU_CHUNK_BYTES - chunkOffset);

		}
		else if (chunkOffset% boundary != 0) {
			// check for correct alignment
			offset += (chunkOffset % boundary);

		}

		uniform->offset = (offset / bytesPerElement);

		offset += (itemSize * bytesPerElement);
	}
	
	return uint32_t(std::ceil(offset / GPU_CHUNK_BYTES) * GPU_CHUNK_BYTES);
}