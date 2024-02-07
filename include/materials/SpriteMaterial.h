#pragma once

#include <memory>

#include <materials/Material.h>
#include <materials/interfaces.h>

namespace mix {

    class SpriteMaterial : public virtual Material,
        public MaterialWithColor,
        public MaterialWithRotation,
        public MaterialWithMap,
        public MaterialWithAlphaMap,
        public MaterialWithSize {

    public:
        [[nodiscard]] std::string type() const override;

        std::shared_ptr<Material> clone() const override;

        static std::shared_ptr<SpriteMaterial> create(const std::unordered_map<std::string, MaterialValue>& values = {});

    protected:
        SpriteMaterial();

        bool setValue(const std::string& key, const MaterialValue& value) override;
    };

}
