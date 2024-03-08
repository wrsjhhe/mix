#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

namespace mix {
	class Node;
	class NodeBuilder;

	typedef std::function<Node* ()> KeywordsCallback;

	class NodeKeywords {
	public:
		std::vector<std::string> keywords;
		std::unordered_map<std::string, Node*> nodes;
		std::unordered_map<std::string,KeywordsCallback> keywordsCallback;

		Node* getNode(const std::string& name);
		NodeKeywords& addKeyword(const std::string& name,const KeywordsCallback& callback);
		std::vector<Node*> parse(const std::string& code);
		void include(NodeBuilder* builder,const std::string& code);

	};
}