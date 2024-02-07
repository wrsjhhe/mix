#pragma once

#include <lights/Light.h>
#include <lights/light_interfaces.h>

namespace mix {

    class PointLight: public Light, public LightWithShadow {

    public:
        float distance;
        float decay;

        [[nodiscard]] std::string type() const override;

        [[nodiscard]] float getPower() const;

        void setPower(float power);

        void dispose() override;

        static std::shared_ptr<PointLight> create(const Color& color = 0xffffff, std::optional<float> intensity = std::nullopt, float distance = 0, float decay = 1);

    protected:
        PointLight(const Color& color, std::optional<float> intensity, float distance, float decay);
    };

}
