#pragma once
#include <vector>

namespace mix {
	class Light;

	class LightsNode {
	public:
		LightsNode* fromLights(const std::vector<Light*>& lights);
	};
}