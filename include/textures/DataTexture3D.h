#pragma once
#include <textures/Texture.h>

namespace mix {

    class DataTexture3D: public Texture {

    public:
        TextureWrapping wrapR{TextureWrapping::ClampToEdge};

        static std::shared_ptr<DataTexture3D> create(
                const std::vector<unsigned char>& data,
                unsigned int width = 1,
                unsigned int height = 1,
                unsigned int depth = 1);

    private:
        DataTexture3D(
                const std::vector<unsigned char>& data,
                unsigned int width,
                unsigned int height,
                unsigned int depth);
    };

}
