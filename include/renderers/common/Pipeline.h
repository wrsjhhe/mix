#pragma once

#include <string>

namespace mix {
	class Pipeline {
	public:
		explicit Pipeline(const std::string& cahceKey):cahceKey(cahceKey){}
		std::string cahceKey;
		uint32_t usedTimes = 0;
	};
}