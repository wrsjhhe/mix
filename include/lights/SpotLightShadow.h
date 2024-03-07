#pragma once

#include <cameras/PerspectiveCamera.h>
#include <lights/LightShadow.h>

namespace mix {

    class SpotLightShadow: public LightShadow {

    public:
        float focus = 1;

        void updateMatrices(Light* light,size_t) override;

        static std::shared_ptr<SpotLightShadow> create();

    protected:
        SpotLightShadow();
    };

}
