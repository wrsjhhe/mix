#include <renderers/common/Backend.h>

using namespace mix;

Backend::Backend(const Parameters& parameters) :parameters(parameters) {

}

std::shared_ptr<Canvas> Backend::getDomElement() {
	return canvas;
}