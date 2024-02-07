#pragma once

#include <lights/Light.h>

#include <optional>

namespace mix {

    class AmbientLight: public Light {

    public:
        [[nodiscard]] std::string type() const override;

        static std::shared_ptr<AmbientLight> create(const Color& color = 0xffffff, std::optional<float> intensity = std::nullopt);

    protected:
        explicit AmbientLight(const Color& color, std::optional<float> intensity);
    };

}
