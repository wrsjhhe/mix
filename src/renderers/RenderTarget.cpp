
#include <renderers/RenderTarget.h>

#include <math/MathUtils.h>

using namespace mix;


std::unique_ptr<RenderTarget> RenderTarget::create(unsigned int width, unsigned int height, const RenderTarget::Options& options) {

	return std::make_unique<RenderTarget>(width, height, options);
}

RenderTarget::RenderTarget(unsigned int width, unsigned int height, const RenderTarget::Options& options)
	: uuid(math::generateUUID()),
	width(width), height(height),
	scissor(0.f, 0.f, (float)width, (float)height),
	viewport(0.f, 0.f, (float)width, (float)height),
	depthBuffer(options.depthBuffer), stencilBuffer(options.stencilBuffer), depthTexture(options.depthTexture) {

	auto texture = Texture::create(std::nullopt);

	if (options.mapping) texture->mapping = *options.mapping;
	if (options.wrapS) texture->wrapS = *options.wrapS;
	if (options.wrapT) texture->wrapT = *options.wrapT;
	if (options.magFilter) texture->magFilter = *options.magFilter;
	if (options.minFilter) texture->minFilter = *options.minFilter;
	if (options.format) texture->format = *options.format;
	if (options.type) texture->type = *options.type;
	if (options.anisotropy) texture->anisotropy = *options.anisotropy;
	if (options.encoding) texture->encoding = *options.encoding;
	if (options.samples) count = *options.samples;
	if (options.count) count = *options.count;

	for (int i = 0; i < count; ++i) {
		textures[i] = texture->clone();
		textures[i]->isRenderTargetTexture = true;
	}
}

void RenderTarget::setSize(unsigned int width, unsigned int height, unsigned int depth) {

	if (this->width != width || this->height != height || this->depth != depth) {

		this->width = width;
		this->height = height;
		this->depth = depth;

		for (int i = 0; i < textures.size(); ++i) {
			this->textures[i]->image->width = width;
			this->textures[i]->image->height = height;
			this->textures[i]->image->depth = depth;
		}
	

		this->dispose();
	}

	this->viewport.set(0, 0, (float)width, (float)height);
	this->scissor.set(0, 0, (float)width, (float)height);
}

std::shared_ptr<Texture> RenderTarget::texture() {
	return textures[0];
}

RenderTarget& RenderTarget::copy(const RenderTarget& source) {

	this->width = source.width;
	this->height = source.height;
	this->depth = source.depth;

	this->viewport.copy(source.viewport);

	this->textures = source.textures;
	//                this->texture.image = { ...this->texture.image }; // See #20328.

	this->depthBuffer = source.depthBuffer;
	this->stencilBuffer = source.stencilBuffer;
	this->depthTexture = source.depthTexture;

	this->samples = source.samples;

	this->count = source.count;

	return *this;
}

void RenderTarget::dispose() {

	if (!disposed) {

		disposed = true;
		this->dispatchEvent("dispose", this);
	}
}

RenderTarget::~RenderTarget() {

	dispose();
}
