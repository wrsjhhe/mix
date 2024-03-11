#pragma once

#include <nodes/core/Node.h>
#include <string>
#include <memory>
namespace mix {
	class UniformGroupNode : public Node {
	public:
		std::string name;
		uint32_t version;
		bool shared;

		UniformGroupNode(const std::string& name,bool shared = false):
			Node("string"),name(name),version(0),shared(shared)
		{}

		virtual std::string type() override {
			return "UniformGroupNode";
		}

		void needsUpdate(bool value) {
			if (value) ++version;
		}

		static std::shared_ptr<UniformGroupNode> uniformGroup(const std::string& name);

		static std::shared_ptr<UniformGroupNode> sharedUniformGroup(const std::string& name);

		static std::shared_ptr<UniformGroupNode> frameGroup;
		static std::shared_ptr<UniformGroupNode> renderGroup;
		static std::shared_ptr<UniformGroupNode> objectGroup;

	};
}