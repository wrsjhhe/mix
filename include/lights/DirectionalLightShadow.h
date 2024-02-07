#pragma once

#include <lights/LightShadow.h>

#include <cameras/OrthographicCamera.h>

namespace mix {

    class DirectionalLightShadow: public LightShadow {

    public:
        static std::shared_ptr<DirectionalLightShadow> create() {

            return std::shared_ptr<DirectionalLightShadow>(new DirectionalLightShadow());
        }

    protected:
        DirectionalLightShadow()
            : LightShadow(OrthographicCamera::create(-5, 5, 5, -5, 0.5f, 500)) {}
    };

}
