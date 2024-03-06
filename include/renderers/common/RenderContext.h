#pragma once
#include <math/Color4.h>
#include <math/Vector4.h>
#include <textures/Texture.h>
#include <vector>

#include <renderers/common/ClippingContext.h>
#include <renderers/RenderTarget.h>

namespace mix {
	struct RenderContext {
		RenderContext();
		unsigned int id;

		bool color = true;
		bool clearColor = true;
		Color4 clearColorValue = Color4(0, 0, 0, 1);

		bool depth = true;
		bool clearDepth = true;
		int clearDepthValue = 1;

		bool stencil = true;
		bool clearStencil = true;
		int clearStencilValue = 1;

		bool viewport = false;
		Vector4 viewportValue;

		bool scissor = false;
		Vector4 scissorValue;

		std::vector<Texture*> textures;
		Texture* depthTexture = nullptr;
		unsigned int activeCubeFace = 0;
		unsigned int sampleCount = 1;

		float width = 0;
		float height = 0;

		uint32_t occlusionQueryCount = 0;

		std::shared_ptr<ClippingContext> clippingContext;

		RenderTarget* renderTarget = nullptr;
		uint32_t activeMipmapLevel = 0;
	};


}