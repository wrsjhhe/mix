#pragma once

#include <lights/Light.h>
#include <lights/light_interfaces.h>

namespace mix {

    class DirectionalLight: public Light, public LightWithShadow, public LightWithTarget {

    public:
        [[nodiscard]] std::string type() const override;

        void dispose() override;

        static std::shared_ptr<DirectionalLight> create(const Color& color = 0xffffff, std::optional<float> intensity = std::nullopt);

    protected:
        DirectionalLight(const Color& color, std::optional<float> intensity);
    };

}
