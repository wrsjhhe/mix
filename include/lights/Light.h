#pragma once

#include <core/Object3D.h>
#include <math/Color.h>

#include <optional>

namespace mix {

    class Light: public Object3D {

    public:
        Color color;
        float intensity;

        Light(const Light&) = delete;

        [[nodiscard]] std::string type() const override;

        virtual void dispose();

    protected:
        Light(const Color& color, std::optional<float> intensity);
    };

}
