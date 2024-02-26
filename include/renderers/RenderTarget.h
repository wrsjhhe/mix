#pragma once

#include <core/EventDispatcher.h>

#include <textures/Texture.h>

#include <math/Vector4.h>

#include <optional>

namespace mix {

    class DepthTexture;

    class RenderTarget : public EventDispatcher {

    public:
        struct Options {

            std::optional<Mapping> mapping;
            std::optional<TextureWrapping> wrapS;
            std::optional<TextureWrapping> wrapT;
            std::optional<Filter> magFilter;
            std::optional<Filter> minFilter;
            std::optional<Format> format;
            std::optional<Type> type;
            std::optional<int> anisotropy;
            std::optional<Encoding> encoding;

            bool generateMipmaps = false;
            bool depthBuffer = true;
            bool stencilBuffer = false;
            std::shared_ptr<DepthTexture> depthTexture;

            std::optional<unsigned int> samples = 0;
            std::optional<unsigned int> count = 1;
        };

        const std::string uuid;

        unsigned int width;
        unsigned int height;
        unsigned int depth = 1;

        unsigned int count = 1;

        unsigned int samples = 0;

        Vector4 scissor;
        bool scissorTest = false;

        Vector4 viewport;

        std::vector<std::shared_ptr<Texture>> textures;

        bool depthBuffer;
        bool stencilBuffer;
        std::shared_ptr<DepthTexture> depthTexture;

        RenderTarget(unsigned int width, unsigned int height, const Options& options);

        RenderTarget(const RenderTarget&) = delete;
        RenderTarget(const RenderTarget&&) = delete;
        RenderTarget operator=(const RenderTarget&) = delete;

        void setSize(unsigned int width, unsigned int height, unsigned int depth = 1);

        std::shared_ptr<Texture> texture();

        RenderTarget& copy(const RenderTarget& source);

        void dispose();

        static std::unique_ptr<RenderTarget> create(unsigned int width, unsigned int height, const Options& options);

        ~RenderTarget() override;

    protected:
        bool disposed = false;

    };

}
