#pragma once

#include <core/BufferGeometry.h>
#include <core/Object3D.h>
#include <materials/SpriteMaterial.h>

namespace mix {

    class Sprite : public Object3D {

    public:
        Vector2 center{ 0.5f, 0.5f };
        std::shared_ptr<SpriteMaterial> material;

        explicit Sprite(const std::shared_ptr<SpriteMaterial>& material);

        [[nodiscard]] std::string type() const override;

        void raycast(Raycaster& raycaster, std::vector<Intersection>& intersects) override;

        BufferGeometry* geometry() override;

        static std::shared_ptr<Sprite> create(const std::shared_ptr<SpriteMaterial>& material = SpriteMaterial::create());

    private:
        std::shared_ptr<BufferGeometry> _geometry;
    };

}
