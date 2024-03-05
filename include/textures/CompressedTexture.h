#pragma once

#include <textures/Texture.h>

namespace mix {

    class CompressedTexture : public Texture {

    public:
        CompressedTexture(int width, int height)
            : Texture(std::nullopt) {
            
            image->width = width;
            image->height = height;
            flipY = false;
            generateMipmaps = false;
        }
    };

}
