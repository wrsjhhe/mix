#pragma once

#include <unordered_map>
#include <memory>
#include <canvas/Canvas.h>

namespace mix {

	class Backend {
	public:
		struct Parameters
		{

		};
		Backend(const Parameters& parameters = {});

		std::shared_ptr<Canvas> getDomElement();

	protected:
		Parameters parameters;
		std::shared_ptr<Canvas> canvas;
	};

}