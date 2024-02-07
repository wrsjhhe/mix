#pragma once

#include <lights/LightShadow.h>

namespace mix {

    class LightWithShadow {

    public:
        std::shared_ptr<LightShadow> shadow;

        virtual ~LightWithShadow() = default;

    protected:
        explicit LightWithShadow(const std::shared_ptr<LightShadow>& shadow): shadow(shadow) {}
    };

    class LightWithTarget {

    public:
        std::shared_ptr<Object3D> target{Object3D::create()};

        virtual ~LightWithTarget() = default;
    };

}
