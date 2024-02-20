#pragma once

#include <math/Color.h>

namespace mix {

    class FogExp2 {

    public:
        Color color;
        float density;

        explicit FogExp2(const Color& hex, float density = 0.00025f);

        bool operator==(const FogExp2& f) const;
    };

}
