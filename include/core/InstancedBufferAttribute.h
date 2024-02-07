#pragma once

#include <core/BufferAttribute.h>

namespace mix {

    template<class T>
    class InstancedBufferAttribute: public TypedBufferAttribute<T> {

    protected:
        int meshPerAttribute;

        InstancedBufferAttribute(std::vector<T> array, int itemSize, bool normalized, int meshPerAttribute = 1)
            : TypedBufferAttribute<T>(array, itemSize, normalized), meshPerAttribute(meshPerAttribute) {}
    };

}
