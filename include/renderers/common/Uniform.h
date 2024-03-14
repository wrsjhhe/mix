#pragma once

#include <string>

namespace mix {
	class Uniform {
	public:
		std::string name;

		uint32_t boundary = 0; // used to build the uniform buffer according to the STD140 layout
		uint32_t itemSize = 0;

		uint32_t offset = 0;// this property is set by WebGPUUniformsGroup and marks the start position in the uniform buffer

		Uniform(const std::string& name,void* value = nullptr) :
			name(name), value(value)
		{
		}

		void setValue(void* value) {
			this->value = value;
		}

		void* getValue() {
			return value;
		}

	protected:
		void* value = nullptr;
	};

	class FloatUniform : public Uniform {
		FloatUniform(const std::string& name, void* value = nullptr) :
			Uniform(name, value)
		{
			boundary = 4;
			itemSize = 1;
		}
	};

	class Vector2Uniform : public Uniform {

		Vector2Uniform(const std::string& name, void* value = nullptr) :Uniform(name, value) {
			boundary = 8;
			itemSize = 2;
		}
	};

	class Vector3Uniform : public Uniform {

		Vector3Uniform(const std::string& name, void* value = nullptr) :Uniform(name, value) {

			boundary = 16;
			itemSize = 3;

		}

	};

	class Vector4Uniform : public Uniform {

		Vector4Uniform(const std::string& name, void* value = nullptr) :Uniform(name, value) {
			boundary = 16;
			itemSize = 4;
		}
	};

	class ColorUniform : public Uniform {
		ColorUniform(const std::string& name, void* value = nullptr) :Uniform(name, value) {
			boundary = 16;
			itemSize = 3;

		}

	};

	class Matrix3Uniform : public Uniform {
		Matrix3Uniform(const std::string& name, void* value = nullptr) :Uniform(name, value) {
			boundary = 48;
			itemSize = 12;
		}
	};

	class Matrix4Uniform : public Uniform {

		Matrix4Uniform(const std::string& name, void* value = nullptr) :Uniform(name, value) {
			boundary = 64;
			itemSize = 16;
		}
	};
}