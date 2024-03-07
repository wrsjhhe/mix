#pragma once

#include <stdint.h>

namespace mix {

	struct Info {

		bool autoReset = true;

		struct Render
		{
			uint32_t calls = 0;
			uint32_t drawCalls = 0;
			uint32_t triangles = 0;
			uint32_t points = 0;
			uint32_t lines = 0;
		};

		struct Timestamp {
			uint32_t compute = 0;
			uint32_t render = 0;
		};

		uint32_t frame = 0;
		uint32_t calls = 0;

		Render render;
		Timestamp timestamp;

		Info();

		~Info();

		void update();

		void reset();
	};

}