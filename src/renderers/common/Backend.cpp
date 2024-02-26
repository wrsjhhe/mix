#include <renderers/common/Backend.h>

using namespace mix;

Backend::Backend(const Parameters& parameters) :parameters(parameters) {

}
Backend::~Backend() {

}

void Backend::init(const std::shared_ptr<Renderer>& renderer) {
	this->renderer = renderer;
}

std::shared_ptr<Canvas> Backend::getDomElement() {
	return canvas;
}