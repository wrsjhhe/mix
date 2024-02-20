#pragma once
#include <math/Color.h>

namespace mix {

    class Fog {

    public:
        Color color;
        float near;
        float far;

        explicit Fog(const Color& color, float near = 1, float far = 1000);

        bool operator==(const Fog& f) const;
    };

}
