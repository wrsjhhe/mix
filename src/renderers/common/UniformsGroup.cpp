#include <renderers/common/UniformsGroup.h>

using namespace mix;

UniformsGroup::UniformsGroup(const std::string& name) :UniformBuffer(name) {

}

UniformsGroup::~UniformsGroup() {

}

UniformsGroup* UniformsGroup::addUniform(const std::shared_ptr<Uniform>& uniform) {
	uniforms.emplace_back(uniform);
	return this;
}

UniformsGroup* UniformsGroup::removeUniform(const std::shared_ptr<Uniform>& uniform) {
	uniforms.erase(std::remove(uniforms.begin(), uniforms.end(), uniform), uniforms.end());
}

std::vector<float> UniformsGroup::buffer() {
	std::vector<float> buffer = _buffer;
	if (buffer.empty()) {
		
		buffer.resize();
	}
}