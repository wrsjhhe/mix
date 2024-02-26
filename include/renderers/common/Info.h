#pragma once

namespace mix {

	struct Info {

		bool autoReset = true;

		struct Render
		{
			unsigned int calls = 0;
			unsigned int drawCalls = 0;
			unsigned int triangles = 0;
			unsigned int points = 0;
			unsigned int lines = 0;
		};

		struct Timestamp {
			unsigned int compute = 0;
			unsigned int render = 0;
		};

		unsigned int frame = 0;
		unsigned int calls = 0;

		Render render;
		Timestamp timestamp;

		Info();

		~Info();

		void update();

		void reset();
	};

}