#include <renderers/common/Buffer.h>
#include <renderers/common/BufferUtils.h>

using namespace mix;

Buffer::Buffer(const std::string& name, const std::vector<float>& buffer) :
	Binding(name),_buffer(buffer)
{

}

Buffer::~Buffer() {

}

uint32_t Buffer::byteLength() const {
	return utils::getFloatLength(_buffer.size() * bytesPerElement);
}

const std::vector<float>& Buffer::buffer() const {
	return _buffer;
}

bool Buffer::update() {
	return true;
}