#pragma once

#include <materials/Material.h>
#include <materials/interfaces.h>

#include <math/Color.h>
#include <textures/Texture.h>

#include <optional>

namespace mix {

    class MeshBasicMaterial
        : public virtual Material,
        public MaterialWithColor,
        public MaterialWithMap,
        public MaterialWithAoMap,
        public MaterialWithLightMap,
        public MaterialWithSpecularMap,
        public MaterialWithAlphaMap,
        public MaterialWithEnvMap,
        public MaterialWithCombine,
        public MaterialWithReflectivity,
        public MaterialWithWireframe,
        public MaterialWithMorphTargets {

    public:
        [[nodiscard]] std::string type() const override;

        std::shared_ptr<Material> clone() const override;

        static std::shared_ptr<MeshBasicMaterial> create(const std::unordered_map<std::string, MaterialValue>& values = {});

        virtual void setDefaultValues() override;
    protected:
        MeshBasicMaterial();

        bool setValue(const std::string& key, const MaterialValue& value) override;
    };

}
