#pragma once

namespace mix::math {

    class ImprovedNoise {

    public:
        [[nodiscard]] float noise(float x, float y, float z) const;
    };

}
