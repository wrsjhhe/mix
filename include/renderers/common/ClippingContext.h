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
		unsigned int version = 0;
		unsigned int globalClippingCount = 0;

		unsigned int localClippingCount = 0;
		bool localClippingEnabled = false;
		bool localClipIntersection = false;

		std::vector<Vector4> planes;

		unsigned int parentVersion = 0;

		Matrix4 viewMatrix;

		ClippingContext();

		void projectPlanes(const std::vector<Plane>& source, int offset);

		void updateGlobal(Renderer* renderer,Camera* camera);

		void update(ClippingContext* parent, NodeMaterial* material);
	};
}