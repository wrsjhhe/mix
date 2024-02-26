#pragma once

#include <constants.h>

#include <core/EventDispatcher.h>

#include <math/Matrix3.h>
#include <math/Vector2.h>

#include <textures/Image.h>

#include <functional>
#include <memory>
#include <optional>
#include <utility>

namespace mix {

    class Texture: public EventDispatcher {

    public:
        inline static Mapping DEFAULT_MAPPING = Mapping::UV;

        unsigned int id = textureId++;

        bool isRenderTargetTexture = false;

        std::string uuid;

        std::string name;

        std::optional<Image> image;
        std::vector<Image> mipmaps;

        std::optional<Mapping> mapping = Texture::DEFAULT_MAPPING;

        TextureWrapping wrapS{TextureWrapping::ClampToEdge};
        TextureWrapping wrapT{TextureWrapping::ClampToEdge};

        Filter magFilter{Filter::Linear};
        Filter minFilter{Filter::LinearMipmapLinear};

        int anisotropy = 1;

        Format format{Format::RGBA};
        std::optional<std::string> internalFormat;
        Type type{Type::UnsignedByte};

        Vector2 offset = Vector2(0, 0);
        Vector2 repeat = Vector2(1, 1);
        Vector2 center = Vector2(0, 0);
        float rotation = 0;

        bool matrixAutoUpdate = true;
        Matrix3 matrix{};

        bool generateMipmaps = true;
        bool premultiplyAlpha = false;
        int unpackAlignment = 4;// valid values: 1, 2, 4, 8 (see http://www.khronos.org/opengles/sdk/docs/man/xhtml/glPixelStorei.xml)

        // Values of encoding !== THREE.LinearEncoding only supported on map, envMap and emissiveMap.
        //
        // Also changing the encoding after already used by a Material will not automatically make the Material
        // update. You need to explicitly call Material.needsUpdate to trigger it to recompile.
        Encoding encoding{Encoding::Linear};

        Texture(const Texture&) = delete;
        Texture operator=(const Texture&) = delete;

        std::optional<std::function<void(Texture&)>> onUpdate;

        void updateMatrix();

        void dispose();

        void transformUv(Vector2& uv) const;

        void needsUpdate();

        [[nodiscard]] unsigned int version() const;

        Texture& copy(const Texture& source);

        [[nodiscard]] std::shared_ptr<Texture> clone() const;

        ~Texture() override;

        static std::shared_ptr<Texture> create(std::optional<Image> image = std::nullopt);

    protected:
        explicit Texture(std::optional<Image> image = std::nullopt);

    private:
        bool disposed_ = false;
        unsigned int version_ = 0;

        inline static unsigned int textureId = 0;
    };

}
