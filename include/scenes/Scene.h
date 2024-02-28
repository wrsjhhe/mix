#pragma once

#include <core/Object3D.h>

#include <scenes/Fog.h>
#include <scenes/FogExp2.h>

#include <memory>
#include <variant>

namespace mix {

    class Texture;
    typedef std::variant<Fog, FogExp2> FogVariant;

    class Scene: public Object3D {

    public:
        [[nodiscard]] std::string type() const override;

        std::optional<Color> background;
        std::shared_ptr<Texture> environment;
        std::optional<FogVariant> fog;

        std::shared_ptr<Material> overrideMaterial;

        bool autoUpdate = true;

        static std::shared_ptr<Scene> create();
    };

}
