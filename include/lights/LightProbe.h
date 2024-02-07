#pragma once

#include <utility>

#include <lights/Light.h>

#include <math/SphericalHarmonics3.h>

namespace mix {

    class LightProbe: public Light {

    public:
        SphericalHarmonis3 sh;

    protected:
        explicit LightProbe(SphericalHarmonis3 sh = SphericalHarmonis3(), float intensity = 1)
            : Light(0xffffff, intensity), sh(std::move(sh)) {}
    };

}
