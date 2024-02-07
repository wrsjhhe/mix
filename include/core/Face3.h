#pragma once

#include <math/Vector3.h>

namespace mix {

    class Face3 {

    public:
        unsigned int a;
        unsigned int b;
        unsigned int c;
        Vector3 normal;

        unsigned int materialIndex;

        Face3(unsigned int a, unsigned int b, unsigned int c, const Vector3& normal, unsigned int materialIndex)
            : a(a), b(b), c(c), normal(normal), materialIndex(materialIndex) {}
    };

}
