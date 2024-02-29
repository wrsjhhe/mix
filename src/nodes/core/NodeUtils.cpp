#include <nodes/core/NodeUtils.h>
#include <nodes/core/Node.h>
using namespace mix;

std::string NodeUtils::getCacheKey(IContainNodes* object) {
	std::string cacheKey = "{";

	auto node = dynamic_cast<Node*>(object);
	if (node != nullptr) {
		cacheKey += node->id;
	}

	auto children = object->getChildren();

	for (auto& [_property, childNode] : children) {

		std::string name = _property.substr(_property.size() - 5, 4);
		cacheKey += ',' + name + ':' + childNode->getCacheKey();

	}

	cacheKey += '}';

	return cacheKey;
}
