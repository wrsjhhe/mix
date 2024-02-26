#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <memory>

namespace mix {
	class Scene;
	class Camera;
	struct RenderContext;
	class RenderTarget;

	class RenderContexts {
	public:
		RenderContext* get(const Scene* scene, const Camera* camera, RenderTarget* renderTarget = nullptr);

		void dispose();

	private:
		std::unordered_map<std::string, std::map<std::pair<const Scene*, const Camera*>,std::shared_ptr<RenderContext>>> chainMaps;
	};
}