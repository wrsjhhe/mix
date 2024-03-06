#include <renderers/webgpu/utils/WebGPUTextureUtils.h>
#include <webgpu/webgpu.hpp>
#include <renderers/WebGPUBackend.h>
#include <textures/DepthTexture.h>
#include <textures/DataTexture3D.h>
#include <textures/FramebufferTexture.h>
#include <textures/CompressedTexture.h>
#include <textures/VideoTexture.h>

#include <webgpu/webgpu.hpp>

using namespace mix;
using namespace wgpu;

static WGPUTextureFormat getFormat(mix::Texture* texture, wgpu::Device* device = nullptr) {
	auto format = texture->format;
	auto type = texture->type;
	auto colorSpace = texture->colorSpace;

	WGPUTextureFormat formatGPU = WGPUTextureFormat_Undefined;

	if (dynamic_cast<FramebufferTexture*>(texture) != nullptr && texture->type == Type::UnsignedByte) {
		formatGPU = WGPUTextureFormat_BGRA8Unorm;
	}
	else if (dynamic_cast<CompressedTexture*>(texture)) {
		std::cerr << "not impl" << std::endl;
	}
	else {

		switch (format)
		{
		case Format::RGBA:
			switch (type)
			{
			case Type::UnsignedByte:
				formatGPU = colorSpace == ColorSpace::SRGBColorSpace ? 
					WGPUTextureFormat_RGBA8UnormSrgb : WGPUTextureFormat_RGBA8Unorm;
				break;
			case Type::HalfFloat:
				formatGPU = WGPUTextureFormat_RGBA16Float;
				break;
			case Type::Float:
				formatGPU = WGPUTextureFormat_RGBA32Float;
				break;
			default:
				std::cerr << "WebGPURenderer: Unsupported texture type with RGBAFormat." << std::endl;
				break;
			}
			break;
		case Format::RGB:
			switch (type)
			{
			case Type::UnsignedByte:
				formatGPU = WGPUTextureFormat_R8Unorm;
				break;
			case Type::HalfFloat:
				formatGPU = WGPUTextureFormat_RGBA16Float;
				break;
			case Type::Float:
				formatGPU = WGPUTextureFormat_RGBA32Float;
				break;
			default:
				std::cerr << "WebGPURenderer: Unsupported texture type with RGBFormat." << std::endl;
				break;
			}
			break;
		case Format::RG:
			switch (type)
			{
			case Type::UnsignedByte:
				formatGPU = WGPUTextureFormat_RG8Unorm;
				break;
			case Type::HalfFloat:
				formatGPU = WGPUTextureFormat_RG16Float;
				break;
			case Type::Float:
				formatGPU = WGPUTextureFormat_RG32Float;
				break;
			default:
				std::cerr << "WebGPURenderer: Unsupported texture type with RGFormat." << std::endl;
				break;
			}
			break;
		case Format::Depth:
			switch (type)
			{
			case Type::UnsignedByte:
				formatGPU = WGPUTextureFormat_Depth16Unorm;
				break;
			case Type::UnsignedInt:
				formatGPU = WGPUTextureFormat_Depth24Plus;
				break;
			case Type::Float:
				formatGPU = WGPUTextureFormat_Depth32Float;
				break;
			default:
				std::cerr << "WebGPURenderer: Unsupported texture type with DepthFormat." << std::endl;
				break;
			}
			break;
		case Format::DepthStencil:
			switch (type)
			{
			case Type::UnsignedInt248:
				formatGPU = WGPUTextureFormat_Depth24PlusStencil8;
				break;
			case Type::Float:
				if (device && !device->hasFeature(WGPUFeatureName_Depth32FloatStencil8)) {			
					std::cerr << "WebGPURenderer: Depth textures with DepthStencilFormat + FloatType can only be used with the \"depth32float - stencil8\" GPU feature." << std::endl;;
				}
				formatGPU = WGPUTextureFormat_Depth32FloatStencil8;
				break;
			default:
				std::cerr << "WebGPURenderer: Unsupported texture type with DepthStencilFormat." << std::endl;
				break;
			}
			break;
		default:
			std::cerr << "WebGPURenderer: Unsupported texture format." << std::endl;
			break;
		}
	}
	return formatGPU;
}

WebGPUTextureUtils::WebGPUTextureUtils(WebGPUBackend* backend):backend(backend) {
	depthTexture = std::make_shared<DepthTexture>();
	depthTexture->name = "depthBuffer";
}

WebGPUTextureUtils::~WebGPUTextureUtils() {
	colorBuffer->destroy();
	colorBuffer = nullptr;
}

wgpu::Texture* WebGPUTextureUtils::getColorBuffer() {
	if (colorBuffer != nullptr) {
		colorBuffer->destroy();
		colorBuffer = nullptr;
	}

	Vector2& vec2 = backend->getDrawingBufferSize();
	uint32_t width = vec2.x;
	uint32_t height = vec2.y;

	TextureDescriptor textureDes{};
	textureDes.label = "colorBuffer";
	textureDes.size = WGPUExtent3D{ width,height,1 };
	textureDes.sampleCount = backend->parameters.sampleCount.value();
	textureDes.format = WGPUTextureFormat::WGPUTextureFormat_BGRA8Unorm;
	textureDes.usage = WGPUTextureUsage::WGPUTextureUsage_RenderAttachment | WGPUTextureUsage::WGPUTextureUsage_CopySrc;
	colorBuffer = std::make_shared<wgpu::Texture>(backend->device->createTexture(textureDes));

	return colorBuffer.get();

}

wgpu::Texture* WebGPUTextureUtils::getDepthBuffer(bool depth, bool stencil) {
	Vector2& vec2 = backend->getDrawingBufferSize();
	uint32_t width = vec2.x;
	uint32_t height = vec2.y;

	auto depthTextureGPU = backend->get(depthTexture.get());

	Format format;
	Type type;

	if (stencil) {
		format = Format::DepthStencil;
		type = Type::UnsignedInt248;
	}
	else if (depth) {
		format = Format::Depth;
		type = Type::UnsignedInt;
	}

	if (depthTextureGPU != nullptr) {

		if (depthTexture->image->width == width && depthTexture->image->height == height 
			&& depthTexture->format == format && depthTexture->type == type) {
			return static_cast<BackendTextureResourceProperties*>(depthTextureGPU.get())->texture.get();
		}
		destroyTexture(depthTexture.get());
	}

	depthTexture->name = "depthBuffer";
	depthTexture->format = format;
	depthTexture->type = type;
	depthTexture->image->width = width;
	depthTexture->image->height = height;

	WebGPUTextureOptions options{};
	options.sampleCount = backend->parameters.sampleCount;
	options.width = width;
	options.height = height;

	createTexture(depthTexture.get(), options);

	return static_cast<BackendTextureResourceProperties*>(depthTextureGPU.get())->texture.get();
}

void WebGPUTextureUtils::createTexture(Texture* texture, WebGPUTextureOptions& options) {
	std::shared_ptr<BackendResourceProperties> data = backend->get(texture);
	if (data == nullptr) {
		data = std::make_shared<BackendTextureResourceProperties>();
		backend->set(texture, data);
	}
	auto textureData = static_cast<BackendTextureResourceProperties*>(data.get());
	if (textureData->initialized) {
		std::cerr << "WebGPUTextureUtils: Texture already initialized." << std::endl;
		return;
	}
	if (!options.needsMipmaps.has_value()) options.needsMipmaps = false;
	if (!options.levels.has_value()) options.levels = 1;
	if (!options.depth.has_value()) options.depth = 1;
	
	uint32_t width = options.width;
	uint32_t height = options.height;
	uint32_t depth = options.depth.value();
	uint32_t levels = options.levels.value();

	WGPUTextureDimension dimension = (WGPUTextureDimension)_getDimension(texture);

	WGPUTextureFormat format = texture->internalFormat.has_value() ? 
		WGPUTextureFormat(texture->internalFormat.value()) : getFormat(texture, backend->device.get());

	uint32_t sampleCount = options.sampleCount.has_value() ? options.sampleCount.value() : 1;

	if (sampleCount > 1) {
		// WebGPU only supports power-of-two sample counts and 2 is not a valid value
		sampleCount = std::pow(2, std::floor(std::log2(sampleCount)));
		if (sampleCount == 2) {
			sampleCount = 4;
		}
	}

	uint32_t primarySampleCount = texture->isRenderTargetTexture ? 1 : sampleCount;

	
	auto usage = TextureUsage::TextureBinding | TextureUsage::CopyDst | TextureUsage::CopySrc;

	if (texture->isStorageTexture) {
		usage |= TextureUsage::StorageBinding;
	}

	if (dynamic_cast<CompressedTexture*>(texture) == nullptr) {
		usage |= TextureUsage::RenderAttachment;
	}

	TextureDescriptor textureDescriptorGPU{};
	textureDescriptorGPU.label = texture->name.c_str();
	textureDescriptorGPU.size = WGPUExtent3D{ width,height,depth };
	textureDescriptorGPU.mipLevelCount = levels;
	textureDescriptorGPU.sampleCount = primarySampleCount;
	textureDescriptorGPU.format = format;
	textureDescriptorGPU.usage = usage;
	textureDescriptorGPU.dimension = dimension;

	if (dynamic_cast<VideoTexture*>(texture)) {
		std::cerr << "not impl VideoTexture" << std::endl;
	}
	else {
		if (format == WGPUTextureFormat_Undefined) {

			std::cerr << "WebGPURenderer: Texture format not supported." << std::endl;

			//return createDefaultTexture(texture);

		}

		textureData->texture = std::make_shared<wgpu::Texture>(backend->device->createTexture(textureDescriptorGPU));
	}

	if (texture->isRenderTargetTexture && sampleCount > 1) {

		auto msaaTextureDescriptorGPU = textureDescriptorGPU;

		msaaTextureDescriptorGPU.label = (std::string(msaaTextureDescriptorGPU.label) + " - msaa").c_str();
		msaaTextureDescriptorGPU.sampleCount = sampleCount;

		textureData->msaaTexture = std::make_shared<wgpu::Texture>(backend->device->createTexture(msaaTextureDescriptorGPU));
	}

	textureData->initialized = true;
	textureData->textureDescriptorGPU = std::make_shared<TextureDescriptor>();
	*textureData->textureDescriptorGPU = textureDescriptorGPU;
}

void WebGPUTextureUtils::destroyTexture(void* texture) {
	auto data = backend->get(texture);
	auto textureData = static_cast<BackendTextureResourceProperties*>(data.get());
	textureData->texture->destroy();

	if (textureData->msaaTexture != nullptr) textureData->msaaTexture->destroy();

	backend->remove(texture);
}

uint32_t WebGPUTextureUtils::_getDimension(Texture* texture) {
	WGPUTextureDimension dimension;
	if (dynamic_cast<DataTexture3D*>(texture) != nullptr) {
		dimension = WGPUTextureDimension_3D;
	}
	else {
		dimension = WGPUTextureDimension_2D;
	}

	return (uint32_t)dimension;
}