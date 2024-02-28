#pragma once

#include <nodes/core/Node.h>
#include <string>

namespace mix {
	class Light;
	class LightNode : public Node {
	public:
		static std::string TARGET_DIRECTION;

		std::string scope;
		Light* light = nullptr;

		LightNode(const std::string& scope = LightNode::TARGET_DIRECTION, Light* light = nullptr);

		void setup();
		
	};
}