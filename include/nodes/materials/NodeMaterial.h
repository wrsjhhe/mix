#pragma once
#include <math/Plane.h>
#include <vector>

namespace mix {
	class NodeMaterial {
	public:
		bool isShadowNodeMaterial = false;

		std::vector<Plane> clippingPlanes;

		bool clipIntersection = false;
	};
}