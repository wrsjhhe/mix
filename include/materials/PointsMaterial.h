#pragma once

#include <materials/Material.h>
#include <materials/interfaces.h>

namespace mix {

    class PointsMaterial : public virtual Material,
        public MaterialWithColor,
        public MaterialWithMap,
        public MaterialWithAlphaMap,
        public MaterialWithSize,
        public MaterialWithMorphTargets {

    public:
        [[nodiscard]] std::string type() const override;

        std::shared_ptr<Material> clone() const override;

        static std::shared_ptr<PointsMaterial> create(const std::unordered_map<std::string, MaterialValue>& values = {});

    protected:
        PointsMaterial();

        bool setValue(const std::string& key, const MaterialValue& value) override;
    };

}
