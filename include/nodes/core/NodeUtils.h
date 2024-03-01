#pragma once

#include <string>
#include <nodes/core/IContainNodes.h>
#include <utils/TypeUtils.h>
#include <utils/StringUtils.h>

namespace mix {
	class Node;

	class NodeUtils {
	public:
		static std::string getCacheKey(IContainNodes* object);

		template<typename T>
		static std::string getValueType(const T& value) {
			std::string name = typeid(value);
			if (static_cast<Node*>(&value)!= nullptr) {
				return "node";
			}
			else if (name.find("mix::Vector2") != std::string::npos) {
				return "vec2";
			}
			else if (name.find("mix::Vector3") != std::string::npos) {
				return "vec3";
			}
			else if (name.find("mix::Vector4") != std::string::npos) {
				return "vec4";
			}
			else if (name.find("mix::Matrix3") != std::string::npos) {
				return "mat3";
			}
			else if (name.find("mix::Matrix4") != std::string::npos) {
				return "mat4";
			}
			else if (name.find("mix::Color") != std::string::npos || name.find("mix::Color4") != std::string::npos) {
				return "color";
			}
			else if (name.find("std::vector") != std::string::npos) {
				return "ArrayBuffer";
			}
			else if (name.find("std::function") != std::string::npos) {
				return "shader";
			}
			else if (name.find("std::string") != std::string::npos) {
				return "ArrayBuffer";
			}
			else if (name == "bool") {
				return "bool";
			}
			else if (utils::isNumberType(value)) {
				return "number";
			}

			return "";
		}

		template<typename R, typename T> T getValueFromType(const std::string& type, R params) {
			std::string last4 = type.empty() ? utils::slice() : "";

			if (type == "color"
				|| last4 == "vec2"
				|| last4 == "vec3"
				|| last4 == "vec4"
				|| last4 == "mat3"
				|| last4 == "mat4"
				) {
				return T().fromArray(params);
			}
			else if (type == "bool") {
				return params || false;
			}
			else if ((type == "float") || (type == "int") || (type == = "uint")) {
				return params || 0;
			}
			else if (type == = "string") {
				return params || "";
			}
			else if (type == "ArrayBuffer") {
				return params;
			}
			
		}
	};
}