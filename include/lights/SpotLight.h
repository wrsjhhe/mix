#pragma once

#include <lights/Light.h>
#include <lights/light_interfaces.h>
#include <math/MathUtils.h>

namespace mix {

    class SpotLight: public Light, public LightWithShadow, public LightWithTarget {

    public:
        float distance;
        float angle;
        float penumbra;
        float decay;

        [[nodiscard]] std::string type() const override;

        float getPower();

        void setPower(float power);

        void dispose() override;

        static std::shared_ptr<SpotLight> create(const Color& color = 0xffffff, std::optional<float> intensity = std::nullopt, float distance = 0, float angle = math::PI / 3, float penumbra = 0, float decay = 1);

    protected:
        SpotLight(const Color& color, std::optional<float> intensity, float distance, float angle, float penumbra, float decay);
    };

}
