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
		{
			static std::function<std::shared_ptr<Node>()> creator = []() {
				return std::shared_ptr<UniformGroupNode>();
			};
			Node::addNodeClass("UniformGroupNode", creator);
		}

		virtual std::string type() override {
			return "UniformGroupNode";
		}

		void needsUpdate(bool value) {
			if (value) ++version;
		}

		static std::shared_ptr<UniformGroupNode> uniformGroup = [](const std::string& name) {
			return std::make_shared<UniformGroupNode>(name);
		};

		static std::shared_ptr<UniformGroupNode> sharedUniformGroup = [](const std::string& name) {
			return std::make_shared<UniformGroupNode>(name,true);
		};

		static std::shared_ptr<UniformGroupNode> frameGroup = sharedUniformGroup("frame");
		static std::shared_ptr<UniformGroupNode> renderGroup = sharedUniformGroup("render");
		static std::shared_ptr<UniformGroupNode> objectGroup = uniformGroup("object");

	};
	Node::addNodeClass("UniformGroupNode", []() {
		return std::shared_ptr<UniformGroupNode>();
		});
}