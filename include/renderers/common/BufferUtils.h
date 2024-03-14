#pragma once

#include <constants.h>

namespace mix {
	namespace utils {
		inline uint32_t getFloatLength(uint32_t floatLength) {
			// ensure chunk size alignment (STD140 layout)
			return floatLength + ((GPU_CHUNK_BYTES - (floatLength % GPU_CHUNK_BYTES)) % GPU_CHUNK_BYTES);
		}

		inline uint32_t getVectorLength(uint32_t count, uint32_t vectorLength = 4) {

			uint32_t strideLength = getStrideLength(vectorLength);

			uint32_t floatLength = strideLength * count;

			return getFloatLength(floatLength);

		}

		inline uint32_t getStrideLength(uint32_t vectorLength) {

			uint32_t strideLength = 4;

			return vectorLength + ((strideLength - (vectorLength % strideLength)) % strideLength);

		}
	}
}