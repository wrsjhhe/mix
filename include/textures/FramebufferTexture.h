#pragma once

#include <textures/Texture.h>

namespace mix {

    class FramebufferTexture : public Texture {

    public:
        FramebufferTexture(int width, int height)
            : Texture(std::nullopt) {
            magFilter = Filter::Nearest;
            minFilter = Filter::Nearest;

            generateMipmaps = false;
        }
    };

}
