#include <nodes/core/UniformGroupNode.h>

using namespace mix;

std::shared_ptr<UniformGroupNode> UniformGroupNode::uniformGroup(const std::string& name) {
	return std::make_shared<UniformGroupNode>(name);
}

std::shared_ptr<UniformGroupNode> UniformGroupNode::sharedUniformGroup(const std::string& name) {
	return std::make_shared<UniformGroupNode>(name, true);
}

std::shared_ptr<UniformGroupNode> UniformGroupNode::frameGroup = UniformGroupNode::sharedUniformGroup("frame");
std::shared_ptr<UniformGroupNode> UniformGroupNode::renderGroup = UniformGroupNode::sharedUniformGroup("render");
std::shared_ptr<UniformGroupNode> UniformGroupNode::objectGroup = UniformGroupNode::uniformGroup("object");