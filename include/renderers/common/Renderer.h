#pragma once

#include <memory>
#include <vector>
#include <math/Plane.h>

namespace mix {
	class Backend;
	class Scene;
	class Camera;
	class Renderer {
	public:
		struct Parameters
		{

		};
		Renderer(const std::shared_ptr<Backend>& backend, const Parameters& parameters = {});

		void init();

		void render(Scene& scene, Camera& camera);

		std::vector<Plane> clippingPlanes;
		bool localClippingEnabled = false;


	private:
		struct Impl;
		std::shared_ptr<Impl> pimpl_;
	};
}