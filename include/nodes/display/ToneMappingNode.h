#pragma once

#include <nodes/core/TempNode.h>
#include <constants.h>

namespace mix {
	class ToneMappingNode : public TempNode {
	private:
		ToneMapping toneMapping;
	public:
		ToneMappingNode(ToneMapping toneMapping = ToneMapping::None);

		virtual std::string type() override {
			return "ToneMappingNode";
		}
	};
}