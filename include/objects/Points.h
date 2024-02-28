#pragma once

#include <core/BufferGeometry.h>
#include <core/Object3D.h>
#include <materials/PointsMaterial.h>
#include <objects/ObjectWithMorphTargetInfluences.h>

namespace mix {

    class Points : public Object3D, public ObjectWithMorphTargetInfluences {

    public:
        Points(std::shared_ptr<BufferGeometry> geometry, std::shared_ptr<Material> material);

        [[nodiscard]] std::string type() const override;

        BufferGeometry* geometry() override;

        Material* material() override;

        std::vector<Material*> materials() override;

        std::shared_ptr<Object3D> clone(bool recursive = true) override;

        void raycast(Raycaster& raycaster, std::vector<Intersection>& intersects) override;

        static std::shared_ptr<Points> create(
            std::shared_ptr<BufferGeometry> geometry = BufferGeometry::create(),
            std::shared_ptr<Material> material = PointsMaterial::create());

    protected:
        std::shared_ptr<BufferGeometry> geometry_;
        std::shared_ptr<Material> material_;
    };

}
