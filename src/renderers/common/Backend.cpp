#include <renderers/common/Backend.h>
#include <renderers/common/Renderer.h>
#include <math/Vector2.h>

using namespace mix;

static Vector2 vector2;

Backend::Backend(const Renderer::Parameters& parameters) :parameters(parameters){

}

Backend::~Backend() {

}

void Backend::init(Renderer* renderer) {
	this->renderer = renderer;
}

Canvas* Backend::getDomElement() {
	return renderer->domElement;
}

Vector2& Backend::getDrawingBufferSize() {
	return renderer->getDrawingBufferSize(vector2);
}

void Backend::set(void* object,const std::shared_ptr<BackendResourceProperties>& value) {
	data[object] = value;
}

std::shared_ptr<BackendResourceProperties> Backend::get(void* object) {
	auto iter = data.find(object);
	if (iter != data.end()) {
		return iter->second;
	}

	return nullptr;
}

bool Backend::has(void* object) {
	auto iter = data.find(object);
	return iter != data.end();
}

void Backend::remove(void* object) {
	auto iter = data.find(object);
	if (iter != data.end()) {
		data.erase(iter);
	}
}