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
		RenderContext* get(Scene* scene, Camera* camera, RenderTarget* renderTarget = nullptr);

		void dispose();

	private:
		std::unordered_map<std::string, std::map<std::pair<Scene*, Camera*>,std::shared_ptr<RenderContext>>> chainMaps;
	};
}