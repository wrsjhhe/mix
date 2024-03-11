#pragma once
#include <vector>
#include <nodes/core/Node.h>
namespace mix {
	class Light;

	class LightsNode : public Node{
	public:
		LightsNode* fromLights(const std::vector<Light*>& lights);

		virtual std::string type() override {
			return "LightsNode";
		}
	};
}