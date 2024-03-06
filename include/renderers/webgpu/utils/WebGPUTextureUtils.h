#pragma once

#include <memory>
#include <optional>

namespace wgpu {
	class Texture;
	class Device;
}

namespace mix {
	class WebGPUBackend;
	class DepthTexture;
	class Texture;
	struct BackendResourceProperties;

	class WebGPUTextureUtils {
	public:
		struct WebGPUTextureOptions {
			uint32_t width;
			uint32_t height;
			std::optional<bool> needsMipmaps;
			std::optional<uint32_t> levels;
			std::optional<uint32_t> depth;
			std::optional<uint32_t> sampleCount;
		};

	public:
		explicit WebGPUTextureUtils(WebGPUBackend* backend);

		~WebGPUTextureUtils();

		wgpu::Texture* getColorBuffer();

		wgpu::Texture* getDepthBuffer(bool depth = true,bool stencil = true);

		void createTexture(Texture* texture, WebGPUTextureOptions& options);

		void destroyTexture(void*);

	private:
		uint32_t _getDimension(Texture* texture);

	private:
		WebGPUBackend* backend = nullptr;

		std::shared_ptr<wgpu::Texture> colorBuffer;
		std::shared_ptr<DepthTexture> depthTexture;
	};
}