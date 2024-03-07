#pragma once
#include <math/Vector4.h>
#include <math/Matrix4.h>
#include <vector>
#include <math/Plane.h>

namespace mix {
	class Renderer;
	class Camera;
	class NodeMaterial;

	class ClippingContext {
	public:
		uint32_t version = 0;
		uint32_t globalClippingCount = 0;

		uint32_t localClippingCount = 0;
		bool localClippingEnabled = false;
		bool localClipIntersection = false;

		std::vector<Vector4> planes;

		uint32_t parentVersion = 0;

		Matrix4 viewMatrix;

		ClippingContext();

		void projectPlanes(const std::vector<Plane>& source, int offset);

		void updateGlobal(Renderer* renderer,Camera* camera);

		void update(ClippingContext* parent, NodeMaterial* material);
	};
}