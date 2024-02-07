#pragma once

#include <lights/Light.h>

namespace mix {

    class HemisphereLight: public Light {

    public:
        Color groundColor;

        [[nodiscard]] std::string type() const override;

        static std::shared_ptr<HemisphereLight> create(const Color& skyColor = 0xffffff, const Color& groundColor = 0xffffff, std::optional<float> intensity = std::nullopt);

    protected:
        HemisphereLight(const Color& skyColor, const Color& groundColor, std::optional<float> intensity);
    };

}
