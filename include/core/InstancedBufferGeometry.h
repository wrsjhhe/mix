#pragma once
#include <core/BufferGeometry.h>

#include <math/infinity.h>

namespace mix {

    class InstancedBufferGeometry: public BufferGeometry {

    public:
        int instanceCount = Infinity<int>;
        int _maxInstanceCount;

        static std::shared_ptr<InstancedBufferGeometry> create() {

            return std::shared_ptr<InstancedBufferGeometry>(new InstancedBufferGeometry());
        }

    protected:
        InstancedBufferGeometry(): BufferGeometry() {}
    };

}
