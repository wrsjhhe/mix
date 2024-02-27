#pragma once
#include <map>
#include <memory>
#include <renderers/common/RenderList.h>

namespace mix {
	class Scene;
	class Camera;

	class RenderLists {
	public:
		RenderList* get(const Scene*, const Camera*);

		void dispose();
	private:
		std::map<std::pair<const Scene*, const Camera*>, std::shared_ptr<RenderList>> lists;
	};
}