#pragma once
#include <map>
#include <memory>
#include <renderers/common/RenderList.h>
#include <utils/ChainMap.h>

namespace mix {
	class Scene;
	class Camera;

	class RenderLists {
	public:
		RenderList* get(const Scene*, const Camera*);

		void dispose();
	private:
		ChainMap lists;
	};
}