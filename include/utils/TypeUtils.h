#pragma once

namespace mix {
	namespace utils {
		template<typename T>
		static bool isNumberType(const T& value) {
			if (
				std::is_same<int8_t, T>::value
				|| std::is_same<int16_t, T>::value
				|| std::is_same<int32_t, T>::value
				|| std::is_same<uint8_t, T>::value
				|| std::is_same<uint16_t, T>::value
				|| std::is_same<uint32_t, T>::value
				|| std::is_same<float, T>::value
				) return true;
			return false;
		}
	}
	
}
