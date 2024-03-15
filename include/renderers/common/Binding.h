#pragma once

#include <string>
#include <memory>

namespace mix {
	class Binding {
	public:
		std::string name;
		
		uint32_t visibility = 0;
		Binding() = default;
		Binding(const std::string& name):name(name){}
		Binding(const Binding& other) {
			name = other.name;
			visibility = other.visibility;
		}
		virtual ~Binding(){}

		void setVisibility(uint32_t visibility) {
			this->visibility |= visibility;
		}
	};
}