#pragma once

#include <textures/Texture.h>

namespace mix {

    class VideoTexture : public Texture {

    protected:
        VideoTexture(int width, int height,
            int type, int mapping,
            int wrapS, int wrapT,
            int magFilter, int minFilter,
            int anisotropy, int format)
            : Texture(std::nullopt) {

            generateMipmaps = false;
        }
    };

}
