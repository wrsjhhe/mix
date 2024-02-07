
#pragma once

#include <textures/Texture.h>

namespace mix {

    class DataTexture: public Texture {

    public:

        void setData(const ImageData& data) {

            image->setData(data);
        }

        static std::shared_ptr<DataTexture> create(
                const ImageData& data,
                unsigned int width = 1, unsigned int height = 1) {
            return std::shared_ptr<DataTexture>(new DataTexture(data, width, height));
        }

    private:
        explicit DataTexture(const ImageData& data, unsigned int width, unsigned int height) {
            this->image = Image{data, width, height};

            this->magFilter = Filter::Nearest;
            this->minFilter = Filter::Nearest;

            this->generateMipmaps = false;
            this->unpackAlignment = 1;

            this->needsUpdate();
        }
    };


}
