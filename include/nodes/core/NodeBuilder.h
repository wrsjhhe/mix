#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <nodes/core/NodeKeywords.h>
#include <nodes/core/NodeCache.h>

namespace mix {
	class Object3D;
	class Renderer;
	class NodeParser;
	class Scene;
	class Material;
	class Node;
	class NodeCache;
	class LightsNode;
	class EnvironmentNode;
	class FogNode;
	class ToneMappingNode;
	class ClippingContext;
	class NodeUniform;
	class StructTypeNode;
	class UniformGroupNode;

	class NodeBuilder {
	public:
		template<typename T>
		struct ShaderContainer {
			std::vector<T> vertex;
			std::vector<T> fragment;
			std::vector<T> compute;
			uint32_t index = 0;
		};

		struct BindingsOffset {
			uint32_t vertex = 0;
			uint32_t fragment = 0;
			uint32_t compute = 0;
		};

		Object3D* object = nullptr;
		Renderer* renderer = nullptr;
		std::shared_ptr<NodeParser> parser = nullptr;
		Scene* scene = nullptr;
		Material* material = nullptr;

		std::vector<Node*> nodes;
		std::vector<Node*> updateNodes;
		std::vector<Node*> updateBeforeNodes;
		std::unordered_map<std::string, Node*> hashNodes;

		LightsNode* lightsNode = nullptr;
		EnvironmentNode* environmentNode = nullptr;
		FogNode* fogNode = nullptr;
		ToneMappingNode* toneMappingNode = nullptr;

		ClippingContext* clippingContext = nullptr;

		std::string vertexShader;
		std::string fragmentShader;
		std::string computeShader;

		ShaderContainer<Object3D*> flowNodes;
		ShaderContainer<std::string> flowCode;
		ShaderContainer<NodeUniform*> uniforms;
		ShaderContainer<StructTypeNode*> structs;
		ShaderContainer<UniformGroupNode*> bindings;

		BindingsOffset bindingsOffset;

		struct Context
		{
			NodeKeywords keywords;
			Material* material;
		};

		Context context;

		/*this.bindingsArray = null;
		this.attributes = [];
		this.bufferAttributes = [];
		this.varyings = [];
		this.codes = {};
		this.vars = {};
		this.flow = { code: '' };
		this.chaining = [];
		this.stack = stack();
		this.stacks = [];
		this.tab = '\t';

		this.currentFunctionNode = null;

		this.flowsData = new WeakMap();

		this.buildStage = null;*/
		std::string shaderStage;
		std::shared_ptr<NodeCache> cache;
		NodeCache* globalCache;

	public:
		NodeBuilder(Object3D* object,Renderer* renderer, std::shared_ptr<NodeParser> parser, Scene* scene = nullptr,Material* material = nullptr);

		template <typename T>
		std::string getTypeFromArray(std::vector<T> array) {
			if (std::is_same<int8_t, T>::value) {
				return "int";
			}
			else if (std::is_same<int16_t, T>::value) {
				return "int";
			}
			else if (std::is_same<int32_t, T>::value) {
				return "int";
			}
			else if (std::is_same<uint8_t, T>::value) {
				return "uint";
			}
			else if (std::is_same<uint16_t, T>::value) {
				return "uint";
			}
			else if (std::is_same<uint32_t, T>::value) {
				return "uint";
			}
			else if (std::is_same<float, T>::value) {
				return "float";
			}
			else {
				throw std::exception("Unsupported data types!");
			}
			
		}

		void setCache(const std::shared_ptr<NodeCache>& cache) {
			this->cache = cache;
		}
		std::shared_ptr<NodeCache> getCache() {
			return cache;
		}
		virtual bool isAvailable(const std::string& name) { return false; }

		NodeStageDataProperties& getNodeProperties(Node* node,const std::string& shaderStage = "any");

		NodeStageData& getDataFromNode(Node* node, std::string shaderStage = "",NodeCache* cache = nullptr);

		void build(bool convertMaterial = true);
	};
}