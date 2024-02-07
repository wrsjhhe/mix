#pragma once

#include <core/BufferAttribute.h>

#include <vector>

namespace mix {

    class InterleavedBuffer: public FloatBufferAttribute {

    public:
        [[nodiscard]] int stride() const {

            return stride_;
        }

        static std::shared_ptr<InterleavedBuffer> create(const std::vector<float>& array, int stride) {

            return std::shared_ptr<InterleavedBuffer>(new InterleavedBuffer(array, stride));
        }

    protected:
        int stride_;

        InterleavedBuffer(const std::vector<float>& array, int stride)
            : TypedBufferAttribute<float>(array, static_cast<int>(array.size() / stride)),
              stride_(stride) {}
    };

}