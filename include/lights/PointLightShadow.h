#pragma once

#include <lights/LightShadow.h>

namespace mix {

    class PointLight;

    class PointLightShadow: public LightShadow {

    public:
        void updateMatrices(PointLight* light, size_t viewportIndex = 0);

        static std::shared_ptr<PointLightShadow> create();

    protected:
        std::vector<Vector3> _cubeDirections;
        std::vector<Vector3> _cubeUps;

        PointLightShadow();
    };

}