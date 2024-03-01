#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

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

	class NodeBuilder {
	public:
		struct ShaderContainer {
			std::vector<std::string> vertex;
			std::vector<std::string> fragment;
			std::vector<std::string> compute;
			uint32_t index = 0;
		};

		struct BindingsOffset {
			uint32_t vertex = 0;
			uint32_t fragment = 0;
			uint32_t compute = 0;
		};
	protected:
		Object3D* object = nullptr;
		Renderer* renderer = nullptr;
		NodeParser* parser = nullptr;
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

		ShaderContainer flowNodes;
		ShaderContainer flowCode;
		ShaderContainer uniforms;
		ShaderContainer structs;
		ShaderContainer bindings;

		BindingsOffset bindingsOffset;
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

		this.context = {
			keywords: new NodeKeywords(),
			material : this.material
		};

		this.cache = new NodeCache();
		this.globalCache = this.cache;

		this.flowsData = new WeakMap();

		this.shaderStage = null;
		this.buildStage = null;*/
		std::shared_ptr<NodeCache> cache;
		NodeCache* globalCache;

	public:
		NodeBuilder(Object3D* object,Renderer* renderer, NodeParser* parser, Scene* scene = nullptr,Material* material = nullptr);

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
	};
}