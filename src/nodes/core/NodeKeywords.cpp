#include <nodes/core/NodeKeywords.h>
#include <nodes/core/Node.h>
#include <regex>

using namespace mix;

Node* NodeKeywords::getNode(const std::string& name) {
	if (!nodes.contains(name)) {
		auto iter = keywordsCallback.find(name);
		if (iter != keywordsCallback.end()) {
			nodes[name] = iter->second();
		}
	}
	auto iter = nodes.find(name);

	return iter != nodes.end() ? iter->second : nullptr;
}

NodeKeywords& NodeKeywords::addKeyword(const std::string& name, const KeywordsCallback& callback) {
	keywords.emplace_back(name);
	keywordsCallback[name] = callback;

	return *this;
}

std::vector<Node*> NodeKeywords::parse(const std::string& code) {

	static std::regex regExp(R"(\\b|\\b' )}\\b)", std::regex_constants::icase);
	std::vector<std::string>& keywordNames = keywords;
	auto strIter = std::sregex_iterator(code.begin(), code.end(), regExp);
	auto strEnd = std::sregex_iterator();
	std::vector<Node*> keywordNodes;
	
	for (; strIter != strEnd; ++strIter) {
		auto iter = strIter->begin();
		for (; iter != strIter->end(); ++iter) {
			if (iter->matched) {
				std::string keyword = iter->str();
				Node* node = getNode(keyword);
				if (node != nullptr && std::find(keywordNodes.begin(), keywordNodes.end(), node) == keywordNodes.end()) {
					keywordNodes.emplace_back(node);
				}
			}
		}
	}

	return keywordNodes;
}

void NodeKeywords::include(NodeBuilder* builder, const std::string& code) {
	std::vector keywordNodes = parse(code);

	for (Node* keywordNode : keywordNodes) {
		keywordNode->build(builder);
	}
}